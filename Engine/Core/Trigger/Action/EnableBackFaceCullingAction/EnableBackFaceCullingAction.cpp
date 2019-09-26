#include "EnableBackFaceCullingAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>


EnableBackFaceCullingAction::EnableBackFaceCullingAction(void)
{}


EnableBackFaceCullingAction::EnableBackFaceCullingAction(int32 _type): AbstractAction(_type)
{}



void EnableBackFaceCullingAction::UpdateValid(void)
{
	valid = false; 

	if(arg.IsNotEmpty() && target.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
		valid = true;
	}
}



EnableBackFaceCullingAction::~EnableBackFaceCullingAction(void)
{
	Destroying();
}


void EnableBackFaceCullingAction::SetVariableArg(int32 _index, StringANSI _name)
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
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ArgIsRenamed));
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
					if(object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS)) 
					{
						if(target.Attach(dynamic_cast<GraphicsSurface*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TargetIsRenamed));
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


void EnableBackFaceCullingAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TemplateArgIsCompleted));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ArgIsLost));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TemplateArgIsChanged));
			
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
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TemplateTargetIsCompleted));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TargetIsLost));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TemplateTargetIsChanged));
			
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


void EnableBackFaceCullingAction::SetBooleanKeyArg(int32 _index, bool _value)
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



void EnableBackFaceCullingAction::TemplateArgIsCompleted(void)
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
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void EnableBackFaceCullingAction::ArgIsLost(void)
{
	if(arg.IsNotEmpty())
	{ 
		arg.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsCreated));
	valid = false;
}



void EnableBackFaceCullingAction::TemplateArgIsChanged(void)
{
	ArgIsLost();

	TemplateArgIsCompleted(); 
}



void EnableBackFaceCullingAction::TemplateTargetIsCompleted(void)
{
	if(templateTarget.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateTarget.GetResult(), state[ASSET_TYPE][TARGET]) : AssetLibrary::_GetAssets().FindObject(templateTarget.GetResult());

		if(target.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS)) 
				{
					if(target.Attach(dynamic_cast<GraphicsSurface*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void EnableBackFaceCullingAction::TargetIsLost(void)
{
	if(target.IsNotEmpty())
	{
		target.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		target.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		target.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsCreated));
	valid = false;
}



void EnableBackFaceCullingAction::TemplateTargetIsChanged(void)
{
	TargetIsLost();

	TemplateTargetIsCompleted(); 
}



StringANSI EnableBackFaceCullingAction::GetTemplateArg(int32 _index)
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



StringANSI EnableBackFaceCullingAction::GetVariableArg(int32 _index)
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



bool EnableBackFaceCullingAction::GetBooleanKeyArg(int32 _index)
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



void EnableBackFaceCullingAction::operator () (void)
{
	if(IsValid()) 
	{
		target.Get()->SetBackFaceCullingEnable(arg.Get()->GetValue());
	}
}



void EnableBackFaceCullingAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsCreated));
	valid = false;
}



void EnableBackFaceCullingAction::TargetIsRenamed(AbstractObject::NameInfo* _info)
{
	TargetIsLost();
}

 

void EnableBackFaceCullingAction::ArgIsRenamed(AbstractObject::NameInfo* _info)
{
	ArgIsLost();
}


void EnableBackFaceCullingAction::ObjectIsCreated(AbstractObject* _object)
{
	if(target.IsEmpty())
	{
		if(state[TEMPLATE][TARGET])
		{
			if(_object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS))
			{ 
				if(templateTarget.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateTarget.GetResult() == _object->GetName())
				{
					if(target.Attach(dynamic_cast<GraphicsSurface*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][TARGET])
		{
			if(_object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS)) 
			{
				if(targetName == _object->GetName())
				{
					if(target.Attach(dynamic_cast<GraphicsSurface*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::TargetIsRenamed));
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
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ArgIsRenamed));
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
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnableBackFaceCullingAction::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool EnableBackFaceCullingAction::SaveToFile(StringANSI _path)			
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


bool EnableBackFaceCullingAction::SaveToFile(File& _file)
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


AbstractAction* EnableBackFaceCullingAction::_LoadFromFile(File& _file)
{
	EnableBackFaceCullingAction* action = new EnableBackFaceCullingAction(GraphicsSurface::ENABLE_BACK_FACE_CULLING_MESSAGE);
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