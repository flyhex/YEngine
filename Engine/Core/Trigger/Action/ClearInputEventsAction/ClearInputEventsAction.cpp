#include "ClearInputEventsAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>


ClearInputEventsAction::ClearInputEventsAction(void)
{}


ClearInputEventsAction::ClearInputEventsAction(int32 _type): AbstractAction(_type)
{}



ClearInputEventsAction::~ClearInputEventsAction(void)
{
	Destroying();
}


void ClearInputEventsAction::UpdateValid(void)
{
	valid = false; 

	if(target.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
		valid = true;
	}
}


void ClearInputEventsAction::SetVariableArg(int32 _index, StringANSI _name)
{
	switch(_index)
	{
		case TARGET:
		{  
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateTarget.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::PLAYER_CONTROL_ASSET;

			TargetIsLost();
  
			targetName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(targetName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(targetName);

			if(target.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(PlayerControl::PLAYER_CONTROL_CLASS)) 
					{
						if(target.Attach(dynamic_cast<PlayerControl*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::TargetIsRenamed));
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


void ClearInputEventsAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case TARGET:
		{
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::TemplateTargetIsCompleted));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::TargetIsLost));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::TemplateTargetIsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::PLAYER_CONTROL_ASSET;

			TargetIsLost(); 

			if(templateTarget.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateTargetIsCompleted();
			}
			break;
		}
	}
}



void ClearInputEventsAction::TemplateTargetIsCompleted(void)
{
	if(templateTarget.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateTarget.GetResult(), state[ASSET_TYPE][TARGET]) : AssetLibrary::_GetAssets().FindObject(templateTarget.GetResult());

		if(target.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(PlayerControl::PLAYER_CONTROL_CLASS)) 
				{
					if(target.Attach(dynamic_cast<PlayerControl*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void ClearInputEventsAction::TargetIsLost(void)
{
	if(target.IsNotEmpty())
	{
		target.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		target.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		target.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::ObjectIsCreated));
	valid = false;
}



void ClearInputEventsAction::TemplateTargetIsChanged(void)
{
	TargetIsLost();

	TemplateTargetIsCompleted(); 
}



StringANSI ClearInputEventsAction::GetTemplateArg(int32 _index)
{
	switch(_index)
	{
		case TARGET:
		{
			return templateTarget.GetStringExpr();
		}
	}
	return "";
}



StringANSI ClearInputEventsAction::GetVariableArg(int32 _index)
{
	switch(_index)
	{
		case TARGET:
		{
			return targetName;
		}
	}
	return "";
}



void ClearInputEventsAction::operator () (void)
{
	if(IsValid()) 
	{
		target.Get()->Destroy();
	}
}



void ClearInputEventsAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::ObjectIsCreated));
	valid = false;
}



void ClearInputEventsAction::TargetIsRenamed(AbstractObject::NameInfo* _info)
{
	TargetIsLost();
}

 

void ClearInputEventsAction::ObjectIsCreated(AbstractObject* _object)
{
	if(target.IsEmpty())
	{
		if(state[TEMPLATE][TARGET])
		{
			if(_object->IsClassPropertyExist(PlayerControl::PLAYER_CONTROL_CLASS)) 
			{ 
				if(templateTarget.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateTarget.GetResult() == _object->GetName())
				{
					if(target.Attach(dynamic_cast<PlayerControl*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][TARGET])
		{
			if(_object->IsClassPropertyExist(PlayerControl::PLAYER_CONTROL_CLASS)) 
			{
				if(targetName == _object->GetName())
				{
					if(target.Attach(dynamic_cast<PlayerControl*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ClearInputEventsAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool ClearInputEventsAction::SaveToFile(StringANSI _path)			
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


bool ClearInputEventsAction::SaveToFile(File& _file)
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


AbstractAction* ClearInputEventsAction::_LoadFromFile(File& _file)
{
	ClearInputEventsAction* action = new ClearInputEventsAction(PlayerControl::CLEAR_INPUT_EVENTS_MESSAGE);
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