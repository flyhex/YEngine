#include "EnableCollisionCheckingAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>


EnableCollisionCheckingAction::EnableCollisionCheckingAction(void)
{}


EnableCollisionCheckingAction::EnableCollisionCheckingAction(int32 _type): AbstractAction(_type)
{}



void EnableCollisionCheckingAction::UpdateValid(void)
{
	valid = false; 

	if(arg.IsNotEmpty() && target.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
		valid = true;
	}
}



EnableCollisionCheckingAction::~EnableCollisionCheckingAction(void)
{
	Destroying();
}


void EnableCollisionCheckingAction::SetVariableArg(int32 _index, StringANSI _name)
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
					if(object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS)) 
					{ 
						if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(arg.Attach(dynamic_cast<Variable<bool>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ArgIsRenamed));
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
			state[ASSET_TYPE][_index] = AssetLibrary::OBJECT_ASSET;

			TargetIsLost();
  
			targetName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(targetName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(targetName);

			if(target.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
					{
						if(target.Attach(dynamic_cast<PolygonalSurface*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TargetIsRenamed));
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


void EnableCollisionCheckingAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TemplateArgIsCompleted));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ArgIsLost));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TemplateArgIsChanged));
			
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
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TemplateTargetIsCompleted));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TargetIsLost));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TemplateTargetIsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::OBJECT_ASSET;

			TargetIsLost(); 

			if(templateTarget.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateTargetIsCompleted();
			}
			break;
		}
	}
}


void EnableCollisionCheckingAction::SetBooleanKeyArg(int32 _index, bool _value)
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
			Variable<bool>* object = new Variable<bool>();
			object->SetValue(_value);
			
			if(arg.Attach(object))
			{
				UpdateValid();
			}
			break;
		}
	}
}



void EnableCollisionCheckingAction::TemplateArgIsCompleted(void)
{
	if(templateArg.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg.GetResult());

		if(arg.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS)) 
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(arg.Attach(dynamic_cast<Variable<bool>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void EnableCollisionCheckingAction::ArgIsLost(void)
{
	if(arg.IsNotEmpty())
	{ 
		arg.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsCreated));
	valid = false;
}



void EnableCollisionCheckingAction::TemplateArgIsChanged(void)
{
	ArgIsLost();

	TemplateArgIsCompleted(); 
}



void EnableCollisionCheckingAction::TemplateTargetIsCompleted(void)
{
	if(templateTarget.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateTarget.GetResult(), state[ASSET_TYPE][TARGET]) : AssetLibrary::_GetAssets().FindObject(templateTarget.GetResult());

		if(target.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
				{
					if(target.Attach(dynamic_cast<PolygonalSurface*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void EnableCollisionCheckingAction::TargetIsLost(void)
{
	if(target.IsNotEmpty())
	{
		target.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		target.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		target.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsCreated));
	valid = false;
}



void EnableCollisionCheckingAction::TemplateTargetIsChanged(void)
{
	TargetIsLost();

	TemplateTargetIsCompleted(); 
}



StringANSI EnableCollisionCheckingAction::GetTemplateArg(int32 _index)
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



StringANSI EnableCollisionCheckingAction::GetVariableArg(int32 _index)
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



bool EnableCollisionCheckingAction::GetBooleanKeyArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			if(arg.IsNotEmpty())
			{
				return arg.Get()->GetValue();
			}
		}
	}
	return false;
}



void EnableCollisionCheckingAction::operator () (void)
{
	if(IsValid()) 
	{ 
		target.Get()->SetCollisionEnable(arg.Get()->GetValue());
	}
}



void EnableCollisionCheckingAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsCreated));
	valid = false;
}



void EnableCollisionCheckingAction::TargetIsRenamed(AbstractObject::NameInfo* _info)
{
	TargetIsLost();
}

 

void EnableCollisionCheckingAction::ArgIsRenamed(AbstractObject::NameInfo* _info)
{
	ArgIsLost();
}


void EnableCollisionCheckingAction::ObjectIsCreated(AbstractObject* _object)
{
	if(target.IsEmpty())
	{
		if(state[TEMPLATE][TARGET])
		{
			if(_object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS))
			{ 
				if(templateTarget.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateTarget.GetResult() == _object->GetName())
				{
					if(target.Attach(dynamic_cast<PolygonalSurface*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][TARGET])
		{
			if(_object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
			{
				if(targetName == _object->GetName())
				{
					if(target.Attach(dynamic_cast<PolygonalSurface*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::TargetIsRenamed));
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
			if(_object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
			{
				if(templateArg.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg.GetResult() == _object->GetName())
				{
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg.Attach(dynamic_cast<Variable<bool>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG1])
		{
			if(_object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
			{
				if(argName == _object->GetName())
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg.Attach(dynamic_cast<Variable<bool>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableCollisionCheckingAction::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool EnableCollisionCheckingAction::SaveToFile(StringANSI _path)			
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


bool EnableCollisionCheckingAction::SaveToFile(File& _file)
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
			_file.Write(GetBooleanKeyArg(ARG1));
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


AbstractAction* EnableCollisionCheckingAction::_LoadFromFile(File& _file)
{
	EnableCollisionCheckingAction* action = new EnableCollisionCheckingAction(PolygonalSurface::ENABLE_COLLISION_CHECKING_MESSAGE);
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
		bool arg; 
		_file.Read(arg); 
		action->SetBooleanKeyArg(ARG1, arg);
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