#include "CreateQuaternionVariableAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>


CreateQuaternionVariableAction::CreateQuaternionVariableAction(void)
{}


CreateQuaternionVariableAction::CreateQuaternionVariableAction(int32 _type): AbstractAction(_type)
{}



CreateQuaternionVariableAction::~CreateQuaternionVariableAction(void)
{
	Destroying();
}


void CreateQuaternionVariableAction::UpdateValid(void)
{
	valid = false; 

	if(arg1.IsNotEmpty() && (arg2.IsNotEmpty() || state[KEY][ARG2] || state[EXPRESSION][ARG2] || state[TEMPLATE][ARG2]))
	{
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
		
		if(state[EXPRESSION][ARG2])
		{
			if(expr.GetCurrentError() == ArithmeticExprParserEx<Quaternion>::NO_ERRORS)
			{
				valid = true;
			}
		}
		else { valid = true; } 
	}
}



void CreateQuaternionVariableAction::SetVariableArg(int32 _index, StringANSI _name)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg1.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg1IsLost(); 
			
			arg1Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg1Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg1Name);

			if(arg1.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
					{
						if(arg1.Attach(dynamic_cast<Variable<StringANSI>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg1IsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		} 

		case ARG2:
		{
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg2.Destroy();

			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg2IsLost(); 
			
			arg2Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg2Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg2Name);

			if(arg2.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
					{
						if(arg2.Attach(dynamic_cast<Variable<Quaternion>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg2IsRenamed));
							UpdateValid();
						} 
						return;
					}
				}
			}
			break;
		}
	}
}


void CreateQuaternionVariableAction::SetExpressionArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG2: 
		{   
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg2.Destroy();  

			SetArgMode(_index, EXPRESSION);

			Arg2IsLost();  

			expr.Parse(_expr);
			
			UpdateValid();
			break;
		}  
	}
}



void CreateQuaternionVariableAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::TemplateArg1IsCompleted));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg1IsLost));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::TemplateArg1IsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg1IsLost(); 

			if(templateArg1.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg1IsCompleted();
			}
			break;
		}  

		case ARG2: 
		{
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::TemplateArg2IsCompleted));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg2IsLost));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::TemplateArg2IsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET; 

			Arg2IsLost(); 

			if(templateArg2.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateArg2IsCompleted();
			}
			break;
		}  
	}
}
 

void CreateQuaternionVariableAction::SetStringKeyArg(int32 _index, StringANSI _value)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg1.Destroy();
			
			SetArgMode(_index, KEY);  

			Arg1IsLost();
			Variable<StringANSI>* object = new Variable<StringANSI>();
			object->SetValue(_value);
			
			if(arg1.Attach(object))
			{
				UpdateValid();
			}
			break;
		}
	}
}



 
void CreateQuaternionVariableAction::SetQuaternionKeyArg(int32 _index, Quaternion _value)
{
	switch(_index)
	{
		case ARG2:  
		{ 
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg2.Destroy(); 

			SetArgMode(_index, KEY); 

			Arg2IsLost();

			arg2Value = _value;
			
			UpdateValid();
			break;
		}
	}
}



void CreateQuaternionVariableAction::TemplateArg2IsCompleted(void)
{
	if(templateArg2.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg2.GetResult(), state[ASSET_TYPE][ARG2]) : AssetLibrary::_GetAssets().FindObject(templateArg2.GetResult());

		if(arg2.IsEmpty())
		{
			if(object)
			{  
				if(object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
				{
					if(arg2.Attach(dynamic_cast<Variable<Quaternion>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg2IsRenamed));
						UpdateValid();
					} 
					return;
				}
			}
		}
	}
}


void CreateQuaternionVariableAction::TemplateArg2IsChanged(void)
{
	Arg2IsLost();

	TemplateArg2IsCompleted(); 
}



void CreateQuaternionVariableAction::Arg2IsLost(void)
{
	if(arg2.IsNotEmpty())
	{ 
		arg2.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg2.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg2.Clear();
	}
	expr.Destroy();
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsCreated));
	valid = false;
}
 

void CreateQuaternionVariableAction::TemplateArg1IsCompleted(void)
{
	if(templateArg1.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg1.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg1.GetResult());

		if(arg1.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
				{
					if(arg1.Attach(dynamic_cast<Variable<StringANSI>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void CreateQuaternionVariableAction::TemplateArg1IsChanged(void)
{
	Arg1IsLost();

	TemplateArg1IsCompleted(); 
}



void CreateQuaternionVariableAction::Arg1IsLost(void)
{
	if(arg1.IsNotEmpty())
	{ 
		arg1.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg1.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg1.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsCreated));
	valid = false;
}



StringANSI CreateQuaternionVariableAction::GetExpressionArg(int32 _index)
{
	switch(_index)
	{
		case ARG2:
		{
			return expr.GetArithmeticExpr();
		}
	}
	return "";
}



StringANSI CreateQuaternionVariableAction::GetTemplateArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return templateArg1.GetStringExpr();
		}

		case ARG2:
		{
			return templateArg2.GetStringExpr();
		}
	}
	return "";
}



StringANSI CreateQuaternionVariableAction::GetVariableArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return arg1Name;
		}

		case ARG2:
		{
			return arg2Name;
		}
	}
	return "";
}



StringANSI CreateQuaternionVariableAction::GetStringKeyArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			if(arg1.IsNotEmpty())
			{
				return arg1.Get()->GetValue();
			}
		}
	}
	return "";
}



Quaternion CreateQuaternionVariableAction::GetQuaternionKeyArg(int32 _index)
{
	switch(_index)
	{
		case ARG2:
		{
			return arg2Value; 
		}
	}
	return Quaternion();
}




void CreateQuaternionVariableAction::operator () (void)
{
	if(IsValid()) 
	{
		if(state[KEY][ARG2])
		{
			Variable<Quaternion>* variable = new Variable<Quaternion>(arg2Value);
				
			variable->Rename(arg1.Get()->GetValue());
			
			if(!AssetLibrary::_GetAssets().AddObject(variable))
			{
				delete variable;
			}
		}
		else if(state[EXPRESSION][ARG2])
		{
			if(expr.Calculate() == ArithmeticExprParser<Quaternion>::NO_ERRORS)
			{  
				Variable<Quaternion>* variable = new Variable<Quaternion>(expr.GetResult());
				
				variable->Rename(arg1.Get()->GetValue());
				
				if(!AssetLibrary::_GetAssets().AddObject(variable))
				{
					delete variable;
				}
			}
		} 
		else
		{
			Variable<Quaternion>* variable = new Variable<Quaternion>(arg2.Get()->GetValue());
				
			variable->Rename(arg1.Get()->GetValue());
			
			if(!AssetLibrary::_GetAssets().AddObject(variable))
			{
				delete variable;
			}
		}
	}
}



void CreateQuaternionVariableAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsCreated));
	valid = false;
}


 

void CreateQuaternionVariableAction::Arg1IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg1IsLost();
}


void CreateQuaternionVariableAction::Arg2IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg2IsLost();
}


void CreateQuaternionVariableAction::ObjectIsCreated(AbstractObject* _object)
{
	if(arg1.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG1])
		{
			if(_object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS))
			{
				if(templateArg1.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg1.GetResult() == _object->GetName())
				{
					if(arg1.Attach(dynamic_cast<Variable<StringANSI>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg1IsRenamed));
						UpdateValid(); 
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG1])
		{
			if(_object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS))
			{
				if(arg1Name == _object->GetName())
				{
					if(arg1.Attach(dynamic_cast<Variable<StringANSI>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg2.IsEmpty())
	{
		if(state[TEMPLATE][ARG2])
		{
			if(_object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
			{ 
				if(templateArg2.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg2.GetResult() == _object->GetName())
				{
					if(arg2.Attach(dynamic_cast<Variable<Quaternion>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG2])  
		{
			if(_object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS))
			{
				if(arg2Name == _object->GetName())
				{
					if(arg2.Attach(dynamic_cast<Variable<Quaternion>*>(_object)))
					{ 
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CreateQuaternionVariableAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool CreateQuaternionVariableAction::SaveToFile(StringANSI _path)			
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + actionsDir + GetName() + "." + actionsSuffix;
	} 

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool CreateQuaternionVariableAction::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(type);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		for(int32 i = 0; i < AbstractAction::WIDTH; i++)
		{
			for(int32 j = 0; j < AbstractAction::HEIGHT; j++)
			{
				_file.Write(state[i][j]);
			}
		}
		_file.Write(activationLimit);
		_file.Write(loadArgsEnable);
		if(state[TEMPLATE][ARG1])
		{
			_file.Write(GetTemplateArg(ARG1).length());
			_file.WriteString(GetTemplateArg(ARG1));
		}
		else if(state[VARIABLE][ARG1])
		{
			_file.Write(GetVariableArg(ARG1).length());
			_file.WriteString(GetVariableArg(ARG1));
		}
		else if(state[KEY][ARG1])
		{
			_file.Write(GetStringKeyArg(ARG1).length());
			_file.WriteString(GetStringKeyArg(ARG1));
		}
		if(state[EXPRESSION][ARG2])
		{
			_file.Write(GetExpressionArg(ARG2).length());
			_file.WriteString(GetExpressionArg(ARG2));
		}
		else if(state[TEMPLATE][ARG2])
		{
			_file.Write(GetTemplateArg(ARG2).length());
			_file.WriteString(GetTemplateArg(ARG2));
		}
		else if(state[VARIABLE][ARG2])
		{
			_file.Write(GetVariableArg(ARG2).length());
			_file.WriteString(GetVariableArg(ARG2));
		}
		else if(state[KEY][ARG2])
		{
			_file.Write(GetQuaternionKeyArg(ARG2));
		} 
		return true;
	}
	return false;
}


AbstractAction* CreateQuaternionVariableAction::_LoadFromFile(File& _file)
{
	CreateQuaternionVariableAction* action = new CreateQuaternionVariableAction(AbstractVariable::CREATE_QUATERNION_VARIABLE_MESSAGE);
	StringANSI name;
	int32 length;
	_file.Read(length);
	_file.ReadString(name, length);  
	action->Rename(name);
	for(int32 i = 0; i < AbstractAction::WIDTH; i++)
	{
		for(int32 j = 0; j < AbstractAction::HEIGHT; j++)
		{
			_file.Read(action->state[i][j]);
		}
	}
	int32 activationLimit;
	_file.Read(activationLimit); 
	action->SetActivationLimit(activationLimit);
	bool loadArgsEnable;
	_file.Read(loadArgsEnable); 
	action->SetLoadArgsEnable(loadArgsEnable);
	if(action->state[TEMPLATE][ARG1])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(ARG1, templateArg); 
	}
	else if(action->state[VARIABLE][ARG1])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG1, variableArg);
	}
	else if(action->state[KEY][ARG1])
	{
		StringANSI arg;
		_file.Read(length);
		_file.ReadString(arg, length);
		action->SetStringKeyArg(ARG1, arg);
	}
	if(action->state[EXPRESSION][ARG2])
	{
		StringANSI expression;
		_file.Read(length);
		_file.ReadString(expression, length);
		action->SetExpressionArg(ARG2, expression); 
	}
	else if(action->state[TEMPLATE][ARG2])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(ARG2, templateArg); 
	}
	else if(action->state[VARIABLE][ARG2])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG2, variableArg);
	}
	else if(action->state[KEY][ARG2])
	{
		Quaternion arg; 
		_file.Read(arg);
		action->SetQuaternionKeyArg(ARG2, arg);
	}
	return action;
}