#include "GetColorHSLAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>


GetColorHSLAction::GetColorHSLAction(void)
{}


GetColorHSLAction::GetColorHSLAction(int32 _type): AbstractAction(_type)
{}



GetColorHSLAction::~GetColorHSLAction(void)
{
	Destroying();
}



void GetColorHSLAction::UpdateValid(void)
{
	valid = false; 

	if(arg.IsNotEmpty() && target.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
		valid = true;
	}
}


void GetColorHSLAction::SetVariableArg(int32 _index, StringANSI _name)
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

			ArgIsLost(); 
			
			argName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(argName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(argName);

			if(arg.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS)) 
					{ 
						if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(arg.Attach(dynamic_cast<ColorableObject*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ArgIsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}

		case TARGET:
		{  
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateTarget.Destroy();
			
			SetArgMode(_index, VARIABLE);

			TargetIsLost();
  
			targetName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(targetName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(targetName);

			if(target.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS)) 
					{ 
						if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(target.Attach(dynamic_cast<ColorableObject*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TargetIsRenamed));
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


void GetColorHSLAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TemplateArgIsCompleted));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ArgIsLost));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TemplateArgIsChanged));
			
			SetArgMode(_index, TEMPLATE);

			ArgIsLost(); 

			if(templateArg.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArgIsCompleted();
			}
			break;
		}  

		case TARGET:
		{
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TemplateTargetIsCompleted));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TargetIsLost));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TemplateTargetIsChanged));
			
			SetArgMode(_index, TEMPLATE); 

			TargetIsLost(); 

			if(templateTarget.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateTargetIsCompleted();
			}
			break;
		}
	}
}




void GetColorHSLAction::TemplateArgIsCompleted(void)
{
	if(templateArg.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg.GetResult());

		if(arg.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS)) 
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(arg.Attach(dynamic_cast<ColorableObject*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void GetColorHSLAction::ArgIsLost(void)
{
	if(arg.IsNotEmpty())
	{ 
		arg.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsCreated));
	valid = false;
}



void GetColorHSLAction::TemplateArgIsChanged(void)
{
	ArgIsLost();

	TemplateArgIsCompleted(); 
}



void GetColorHSLAction::TemplateTargetIsCompleted(void)
{
	if(templateTarget.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateTarget.GetResult(), state[ASSET_TYPE][TARGET]) : AssetLibrary::_GetAssets().FindObject(templateTarget.GetResult());

		if(target.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][TARGET] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(target.Attach(dynamic_cast<ColorableObject*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void GetColorHSLAction::TargetIsLost(void)
{
	if(target.IsNotEmpty())
	{
		target.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		target.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		target.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsCreated));
	valid = false;
}



void GetColorHSLAction::TemplateTargetIsChanged(void)
{
	TargetIsLost();

	TemplateTargetIsCompleted(); 
}



StringANSI GetColorHSLAction::GetTemplateArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return templateArg.GetStringExpr();
		}

		case TARGET:
		{
			return templateTarget.GetStringExpr();
		}
	}
	return "";
}



StringANSI GetColorHSLAction::GetVariableArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return argName;
		}

		case TARGET:
		{
			return targetName;
		}
	}
	return "";
}



void GetColorHSLAction::operator () (void)
{
	if(IsValid()) 
	{ 
		bool targetIsVector3 = false, argIsVector3 = false;

		if(target.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
		{
			targetIsVector3 = true;
		}
		
		if(arg.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
		{
			argIsVector3 = true;
		}

		if(targetIsVector3 && argIsVector3)
		{
			arg.Get()->SetColorRGB(target.Get()->GetColorHSL());
		}
		else if(argIsVector3)
		{
			arg.Get()->SetColorRGB(target.Get()->GetColorHSL());
		}
		else if(targetIsVector3)
		{
			arg.Get()->SetColorHSL(target.Get()->GetColorRGB());
		}
		else
		{
			arg.Get()->SetColorRGB(target.Get()->GetColorRGB());
		}
	}
}



void GetColorHSLAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsCreated));
	valid = false;
}



void GetColorHSLAction::TargetIsRenamed(AbstractObject::NameInfo* _info)
{
	TargetIsLost();
}

 

void GetColorHSLAction::ArgIsRenamed(AbstractObject::NameInfo* _info)
{
	ArgIsLost();
}


void GetColorHSLAction::ObjectIsCreated(AbstractObject* _object)
{
	if(target.IsEmpty())
	{
		if(state[TEMPLATE][TARGET])
		{
			if(_object->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
			{ 
				if(templateTarget.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateTarget.GetResult() == _object->GetName())
				{   
					if(loadArgsEnable) { state[ASSET_TYPE][TARGET] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(target.Attach(dynamic_cast<ColorableObject*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][TARGET])
		{
			if(_object->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
			{
				if(targetName == _object->GetName())
				{
					if(loadArgsEnable) { state[ASSET_TYPE][TARGET] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(target.Attach(dynamic_cast<ColorableObject*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &GetColorHSLAction::TargetIsRenamed));
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
			if(_object->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS)) 
			{
				if(templateArg.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg.GetResult() == _object->GetName())
				{
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg.Attach(dynamic_cast<ColorableObject*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG1])
		{
			if(_object->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
			{
				if(argName == _object->GetName())
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg.Attach(dynamic_cast<ColorableObject*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &GetColorHSLAction::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool GetColorHSLAction::SaveToFile(StringANSI _path)			
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


bool GetColorHSLAction::SaveToFile(File& _file)
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
		if(state[TEMPLATE][TARGET])
		{
			_file.Write(GetTemplateArg(TARGET).length());
			_file.WriteString(GetTemplateArg(TARGET));
		}
		else if(state[VARIABLE][TARGET])
		{
			_file.Write(GetVariableArg(TARGET).length());
			_file.WriteString(GetVariableArg(TARGET));
		} 
		return true;
	}
	return false;
}


AbstractAction* GetColorHSLAction::_LoadFromFile(File& _file)
{
	GetColorHSLAction* action = new GetColorHSLAction(ColorableObject::GET_COLOR_HSL_MESSAGE);
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
	if(action->state[TEMPLATE][TARGET])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(TARGET, templateArg); 
	}
	else if(action->state[VARIABLE][TARGET])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(TARGET, variableArg);
	}
	return action;
}