#include "ChangeVector3ValueEvent.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>




ChangeVector3ValueEvent::ChangeVector3ValueEvent(void)
{}



ChangeVector3ValueEvent::ChangeVector3ValueEvent(int32 _type): AbstractEvent(_type)
{}



ChangeVector3ValueEvent::~ChangeVector3ValueEvent(void)
{
	Destroying();
}


void ChangeVector3ValueEvent::UpdateValid(void)
{
	if((arg.IsNotEmpty() || state[KEY][ARG1] || state[EXPRESSION][ARG1] || state[TEMPLATE][ARG1] || state[EQUALITY][ARG1] || !argsEnable) && source.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
	}
}




void ChangeVector3ValueEvent::SetVariableArg(int32 _index, StringANSI _name)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg.Destroy();

			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			ArgIsLost(); 
			
			argName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(argName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(argName);

			if(arg.IsEmpty())
			{ 
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)) 
					{
						if(arg.Attach(dynamic_cast<Variable<Vector3>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ArgIsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}

		case SOURCE:
		{
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateSource.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			SourceIsLost();
  
			sourceName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(sourceName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(sourceName);

			if(source.IsEmpty())
			{
				if(object)
				{ 
					if(object->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)) 
					{
						if(source.Attach(dynamic_cast<Variable<Vector3>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::SourceIsRenamed));
							object->Connect(Variable<Vector3>::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::EventHandler));
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


void ChangeVector3ValueEvent::SetExpressionArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{    
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg.Destroy(); 

			SetArgMode(_index, EXPRESSION);

			ArgIsLost();  

			expr.Parse(_expr);
			
			UpdateValid();
			break;
		}   
	}
}


void ChangeVector3ValueEvent::SetEqualityArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{    
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg.Destroy(); 

			SetArgMode(_index, EQUALITY);

			ArgIsLost();  

			equalityArg.Parse(_expr);
			
			UpdateValid();
			break;
		}   
	}
}


void ChangeVector3ValueEvent::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::TemplateArgIsCompleted));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ArgIsLost));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::TemplateArgIsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET; 

			ArgIsLost(); 

			if(templateArg.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateArgIsCompleted();
			}
			break;
		}

		case SOURCE:
		{
			templateSource.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::TemplateSourceIsCompleted));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::SourceIsLost));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::TemplateSourceIsChanged));
			
			SetArgMode(_index, TEMPLATE); 
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			SourceIsLost(); 

			if(templateSource.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateSourceIsCompleted();
			}
			break;
		}
	}
}



void ChangeVector3ValueEvent::SetVector3KeyArg(int32 _index, Vector3 _value)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg.Destroy(); 

			SetArgMode(_index, KEY);

			ArgIsLost();

			argValue = _value;
			
			UpdateValid();
			break;
		}
	}
}



void ChangeVector3ValueEvent::TemplateArgIsCompleted(void)
{
	if(templateArg.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg.GetResult());

		if(arg.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)) 
				{
					if(arg.Attach(dynamic_cast<Variable<Vector3>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void ChangeVector3ValueEvent::TemplateArgIsChanged(void)
{
	ArgIsLost();

	TemplateArgIsCompleted(); 
}



void ChangeVector3ValueEvent::ArgIsLost(void)
{
	if(arg.IsNotEmpty())
	{
		arg.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg.Clear();
	}
	expr.Destroy();
	equalityArg.Destroy();
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsCreated));
}





void ChangeVector3ValueEvent::TemplateSourceIsCompleted(void)
{
	if(templateSource.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateSource.GetResult(), state[ASSET_TYPE][SOURCE]) : AssetLibrary::_GetAssets().FindObject(templateSource.GetResult());

		if(source.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)) 
				{
					if(source.Attach(dynamic_cast<Variable<Vector3>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::SourceIsRenamed));
						object->Connect(Variable<Vector3>::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void ChangeVector3ValueEvent::TemplateSourceIsChanged(void)
{
	SourceIsLost();

	TemplateSourceIsCompleted(); 
}




void ChangeVector3ValueEvent::SourceIsLost(void)
{
	if(source.IsNotEmpty())
	{
		source.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		source.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		source.Get()->Disconnect(Variable<Vector3>::CHANGE_VALUE_MESSAGE, this);
		source.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsCreated));
}



StringANSI ChangeVector3ValueEvent::GetExpressionArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return expr.GetArithmeticExpr();
		}
	}
	return "";
}



StringANSI ChangeVector3ValueEvent::GetEqualityArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return equalityArg.GetEqualityExpr();
		}
	}
	return "";
}



StringANSI ChangeVector3ValueEvent::GetTemplateArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return templateArg.GetStringExpr();
		}

		case SOURCE:
		{
			return templateSource.GetStringExpr();
		}
	}
	return "";
}


StringANSI ChangeVector3ValueEvent::GetVariableArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return argName;
		}

		case SOURCE:
		{
			return sourceName;
		}
	}
	return "";
}


Vector3 ChangeVector3ValueEvent::GetVector3KeyArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return argValue; 
		}
	}
	return Vector3();
}



void ChangeVector3ValueEvent::SetValue(bool _value)
{
	AbstractEvent::SetValue(_value);

	if(_value)
	{
		if(deactivationMode == AbstractEvent::AUTO_SWITCH_OFF) 
		{ 
			SetValue(false); 
		}
	}
}


void ChangeVector3ValueEvent::EventHandler(AbstractVariable* _object)
{
	if(argsEnable)
	{
		if(state[KEY][ARG1])
		{
			if(source.Get()->GetValue() == argValue)
			{ 
				SetValue(true);
			}
			else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
			{
				SetValue(false);
			}
		}
		else if(state[EXPRESSION][ARG1])
		{
			if(expr.Calculate() == ArithmeticExprParserEx<Vector3>::NO_ERRORS)
			{  
				if(source.Get()->GetValue() == expr.GetResult())
				{
					SetValue(true);
				}
				else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
				{
					SetValue(false);
				}
			}
		}
		else if(state[EQUALITY][ARG1])
		{
			if(equalityArg.Calculate() == EqualityExprParserEx::NO_ERRORS)
			{  
				if(equalityArg.GetResult())
				{
					SetValue(true);
				}
				else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
				{
					SetValue(false);
				}
			}
		}
		else
		{
			if(arg.IsNotEmpty())
			{ 
				if(source.Get()->GetValue() == arg.Get()->GetValue())
				{
					SetValue(true);
				}
				else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
				{
					SetValue(false);
				}
			}
		}
	}
	else
	{
		SetValue(true);
	}
}


void ChangeVector3ValueEvent::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsCreated));
}


void ChangeVector3ValueEvent::SourceIsRenamed(AbstractObject::NameInfo* _info)
{
	SourceIsLost();
}
 


void ChangeVector3ValueEvent::ArgIsRenamed(AbstractObject::NameInfo* _info)
{
	ArgIsLost();
}


void ChangeVector3ValueEvent::ObjectIsCreated(AbstractObject* _object)
{
	if(source.IsEmpty())
	{
		if(state[TEMPLATE][SOURCE])
		{
			if(_object->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)) 
			{
				if(templateSource.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateSource.GetResult() == _object->GetName())
				{
					if(source.Attach(dynamic_cast<Variable<Vector3>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::SourceIsRenamed));
						_object->Connect(Variable<Vector3>::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][SOURCE])
		{
			if(_object->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)) 
			{
				if(sourceName == _object->GetName())
				{
					if(source.Attach(dynamic_cast<Variable<Vector3>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::SourceIsRenamed));
						_object->Connect(Variable<Vector3>::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG1])
		{   
			if(_object->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)) 
			{
				if(templateArg.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg.GetResult() == _object->GetName())
				{
					if(arg.Attach(dynamic_cast<Variable<Vector3>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG1])
		{
			if(_object->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS)) 
			{
				if(argName == _object->GetName())
				{
					if(arg.Attach(dynamic_cast<Variable<Vector3>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeVector3ValueEvent::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool ChangeVector3ValueEvent::SaveToFile(StringANSI _path)			
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + eventsDir + GetName() + "." + eventsSuffix;
	} 
 
	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool ChangeVector3ValueEvent::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(type);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		for(int32 i = 0; i < AbstractEvent::WIDTH; i++)
		{
			for(int32 j = 0; j < AbstractEvent::HEIGHT; j++)
			{
				_file.Write(state[i][j]);
			}
		}
		_file.Write(activationLimit);
		_file.Write(loadArgsEnable);
		_file.Write(deactivationMode);
		_file.Write(argsEnable);
		_file.Write(value);
		if(state[EXPRESSION][ARG1])
		{
			_file.Write(GetExpressionArg(ARG1).length());
			_file.WriteString(GetExpressionArg(ARG1));
		}
		else if(state[EQUALITY][ARG1])
		{
			_file.Write(GetEqualityArg(ARG1).length());
			_file.WriteString(GetEqualityArg(ARG1));
		}
		else if(state[TEMPLATE][ARG1])
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
			_file.Write(GetVector3KeyArg(ARG1));
		}
		if(state[TEMPLATE][SOURCE])
		{
			_file.Write(GetTemplateArg(SOURCE).length());
			_file.WriteString(GetTemplateArg(SOURCE));
		}
		else if(state[VARIABLE][SOURCE])
		{
			_file.Write(GetVariableArg(SOURCE).length());
			_file.WriteString(GetVariableArg(SOURCE));
		} 
		return true;
	}
	return false;
}


AbstractEvent* ChangeVector3ValueEvent::_LoadFromFile(File& _file)
{
	ChangeVector3ValueEvent* event = new ChangeVector3ValueEvent(Variable<Vector3>::CHANGE_VECTOR_3_VALUE_MESSAGE);
	StringANSI name;
	int32 length;
	_file.Read(length);
	_file.ReadString(name, length);
	event->Rename(name);
	for(int32 i = 0; i < AbstractEvent::WIDTH; i++)
	{
		for(int32 j = 0; j < AbstractEvent::HEIGHT; j++)
		{
			_file.Read(event->state[i][j]);
		}
	}
	int32 activationLimit;
	_file.Read(activationLimit); 
	event->SetActivationLimit(activationLimit);
	bool loadArgsEnable;
	_file.Read(loadArgsEnable); 
	event->SetLoadArgsEnable(loadArgsEnable);
	int32 deactivationMode;
	_file.Read(deactivationMode);
	event->SetDeactivationMode(deactivationMode);
	bool argsEnable;
	_file.Read(argsEnable);
	event->SetArgsEnable(argsEnable);
	bool value;
	_file.Read(value);
	if(event->state[EXPRESSION][ARG1])
	{
		StringANSI expression;
		_file.Read(length);
		_file.ReadString(expression, length);
		event->SetExpressionArg(ARG1, expression); 
	}
	else if(event->state[EQUALITY][ARG1])
	{
		StringANSI equality;
		_file.Read(length);
		_file.ReadString(equality, length);
		event->SetEqualityArg(ARG1, equality); 
	}
	else if(event->state[TEMPLATE][ARG1])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(ARG1, templateArg); 
	}
	else if(event->state[VARIABLE][ARG1])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(ARG1, variableArg);
	}
	else if(event->state[KEY][ARG1])
	{
		Vector3 arg; 
		_file.Read(arg); 
		event->SetVector3KeyArg(ARG1, arg);
	}
	if(event->state[TEMPLATE][SOURCE])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(SOURCE, templateArg); 
	}
	else if(event->state[VARIABLE][SOURCE])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(SOURCE, variableArg);
	}

	if(value) { event->value = value; }
	return event;
}