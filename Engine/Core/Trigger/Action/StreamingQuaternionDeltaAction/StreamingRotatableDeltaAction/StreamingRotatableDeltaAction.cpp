#include "StreamingRotatableDeltaAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>


StreamingRotatableDeltaAction::StreamingRotatableDeltaAction(void)
{}


StreamingRotatableDeltaAction::StreamingRotatableDeltaAction(int32 _type): AbstractAction(_type)
{}



StreamingRotatableDeltaAction::~StreamingRotatableDeltaAction(void)
{
	Destroying();
}



void StreamingRotatableDeltaAction::UpdateValid(void)
{
	valid = false; 

	if(arg1.IsNotEmpty() && arg2.IsNotEmpty() && arg3.IsNotEmpty() && target.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
		valid = true;
	}
}


void StreamingRotatableDeltaAction::SetVariableArg(int32 _index, StringANSI _name)
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

			Arg1IsLost(); 
			
			arg1Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg1Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg1Name);

			if(arg1.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS)) 
					{ 
						if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(arg1.Attach(dynamic_cast<Variable<bool>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg1IsRenamed));
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

			Arg2IsLost(); 
			
			arg2Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg2Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg2Name);

			if(arg2.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS)) 
					{ 
						if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(arg2.Attach(dynamic_cast<Variable<bool>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg2IsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}
		
		case ARG3:
		{
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg3.Destroy();
			
			SetArgMode(_index, VARIABLE);

			Arg3IsLost(); 
			
			arg3Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg3Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg3Name);

			if(arg3.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS))
					{  
						if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(arg3.Attach(dynamic_cast<RotatableObject*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg3IsRenamed));
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
			state[ASSET_TYPE][_index] = AssetLibrary::QUATERNION_LERP_ASSET;

			TargetIsLost();
  
			targetName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(targetName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(targetName);

			if(target.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(QuaternionLerp::QUATERNION_LERP_CLASS)) 
					{
						if(target.Attach(dynamic_cast<QuaternionLerp*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TargetIsRenamed));
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


void StreamingRotatableDeltaAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TemplateArg1IsCompleted));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg1IsLost));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TemplateArg1IsChanged));
			
			SetArgMode(_index, TEMPLATE);

			Arg1IsLost(); 

			if(templateArg1.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg1IsCompleted();
			}
			break;
		}  

		case ARG2:
		{
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TemplateArg2IsCompleted));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg2IsLost));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TemplateArg2IsChanged));
			
			SetArgMode(_index, TEMPLATE);

			Arg2IsLost(); 

			if(templateArg2.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg2IsCompleted();
			}
			break;
		}

		case ARG3:
		{
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TemplateArg3IsCompleted));
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg3IsLost));
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TemplateArg3IsChanged));
			
			SetArgMode(_index, TEMPLATE);

			Arg3IsLost(); 

			if(templateArg3.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg3IsCompleted();
			}
			break;
		}

		case TARGET:
		{
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TemplateTargetIsCompleted));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TargetIsLost));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TemplateTargetIsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::QUATERNION_LERP_ASSET;

			TargetIsLost(); 

			if(templateTarget.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateTargetIsCompleted();
			}
			break;
		}
	}
}



void StreamingRotatableDeltaAction::SetBooleanKeyArg(int32 _index, bool _value)
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
			Variable<bool>* object = new Variable<bool>();
			object->SetValue(_value);
			
			if(arg1.Attach(object))
			{
				UpdateValid();
			}
			break;
		}

		case ARG2:
		{
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg2.Destroy();
			
			SetArgMode(_index, KEY);

			Arg2IsLost();
			Variable<bool>* object = new Variable<bool>();
			object->SetValue(_value);
			
			if(arg2.Attach(object))
			{
				UpdateValid();
			}
			break;
		}
	}
}



void StreamingRotatableDeltaAction::TemplateArg1IsCompleted(void)
{
	if(templateArg1.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg1.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg1.GetResult());

		if(arg1.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS)) 
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(arg1.Attach(dynamic_cast<Variable<bool>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void StreamingRotatableDeltaAction::TemplateArg2IsCompleted(void)
{
	if(templateArg2.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg2.GetResult(), state[ASSET_TYPE][ARG2]) : AssetLibrary::_GetAssets().FindObject(templateArg2.GetResult());

		if(arg2.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG2] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(arg2.Attach(dynamic_cast<Variable<bool>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void StreamingRotatableDeltaAction::TemplateArg3IsCompleted(void)
{
	if(templateArg3.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg3.GetResult(), state[ASSET_TYPE][ARG3]) : AssetLibrary::_GetAssets().FindObject(templateArg3.GetResult());

		if(arg3.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS)) 
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG3] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(arg3.Attach(dynamic_cast<RotatableObject*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void StreamingRotatableDeltaAction::Arg1IsLost(void)
{
	if(arg1.IsNotEmpty())
	{ 
		arg1.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg1.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg1.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsCreated));
	valid = false;
}



void StreamingRotatableDeltaAction::Arg2IsLost(void)
{
	if(arg2.IsNotEmpty())
	{ 
		arg2.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg2.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg2.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsCreated));
	valid = false;
}


void StreamingRotatableDeltaAction::Arg3IsLost(void)
{
	if(arg3.IsNotEmpty())
	{ 
		if(target.IsNotEmpty())
		{
			target.Get()->Disconnect(QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE, arg3.Get());
		}
		arg3.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg3.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg3.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsCreated));
	valid = false;
}


void StreamingRotatableDeltaAction::TemplateArg1IsChanged(void)
{
	Arg1IsLost();

	TemplateArg1IsCompleted(); 
}


void StreamingRotatableDeltaAction::TemplateArg2IsChanged(void)
{
	Arg2IsLost();

	TemplateArg2IsCompleted(); 
}



void StreamingRotatableDeltaAction::TemplateArg3IsChanged(void)
{
	Arg3IsLost();

	TemplateArg3IsCompleted(); 
}



void StreamingRotatableDeltaAction::TemplateTargetIsCompleted(void)
{
	if(templateTarget.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateTarget.GetResult(), state[ASSET_TYPE][TARGET]) : AssetLibrary::_GetAssets().FindObject(templateTarget.GetResult());

		if(target.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(QuaternionLerp::QUATERNION_LERP_CLASS)) 
				{
					if(target.Attach(dynamic_cast<QuaternionLerp*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void StreamingRotatableDeltaAction::TargetIsLost(void)
{
	if(target.IsNotEmpty())
	{
		if(arg3.IsNotEmpty())
		{
			target.Get()->Disconnect(QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE, arg3.Get());
		}
		target.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		target.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		target.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsCreated));
	valid = false;
}



void StreamingRotatableDeltaAction::TemplateTargetIsChanged(void)
{
	TargetIsLost();

	TemplateTargetIsCompleted(); 
}



StringANSI StreamingRotatableDeltaAction::GetTemplateArg(int32 _index)
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

		case ARG3:
		{
			return templateArg3.GetStringExpr();
		}

		case TARGET:
		{
			return templateTarget.GetStringExpr();
		}
	}
	return "";
}



StringANSI StreamingRotatableDeltaAction::GetVariableArg(int32 _index)
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

		case ARG3:
		{
			return arg3Name;
		}

		case TARGET:
		{
			return targetName;
		}
	}
	return "";
}



bool StreamingRotatableDeltaAction::GetBooleanKeyArg(int32 _index)
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
		
		case ARG2:
		{
			if(arg2.IsNotEmpty())
			{
				return arg2.Get()->GetValue();
			}
		}
	}
	return false;
}


void StreamingRotatableDeltaAction::operator () (void)
{
	if(IsValid()) 
	{ 
		if(arg2.Get()->GetValue())
		{
			if(arg1.Get()->GetValue())
			{
				target.Get()->Connect(QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE, arg3.Get(), Caller<>(arg3.Get(), &RotatableObject::DisplaceRotation));
			}
			else
			{
				target.Get()->Disconnect(QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE, arg3.Get());
			}
		}
		else
		{
			if(arg1.Get()->GetValue())
			{
				target.Get()->Connect(QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE, arg3.Get(), Caller<>(arg3.Get(), &RotatableObject::SetRotation));
			}
			else
			{
				target.Get()->Disconnect(QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE, arg3.Get());
			}
		}
	}
}



void StreamingRotatableDeltaAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsCreated));
	valid = false;
}



void StreamingRotatableDeltaAction::TargetIsRenamed(AbstractObject::NameInfo* _info)
{
	TargetIsLost();
}

 

void StreamingRotatableDeltaAction::Arg1IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg1IsLost();
}



void StreamingRotatableDeltaAction::Arg2IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg2IsLost();
}


void StreamingRotatableDeltaAction::Arg3IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg3IsLost();
}



void StreamingRotatableDeltaAction::ObjectIsCreated(AbstractObject* _object)
{
	if(target.IsEmpty())
	{
		if(state[TEMPLATE][TARGET])
		{
			if(_object->IsClassPropertyExist(QuaternionLerp::QUATERNION_LERP_CLASS)) 
			{ 
				if(templateTarget.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateTarget.GetResult() == _object->GetName())
				{
					if(target.Attach(dynamic_cast<QuaternionLerp*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][TARGET])
		{
			if(_object->IsClassPropertyExist(QuaternionLerp::QUATERNION_LERP_CLASS)) 
			{
				if(targetName == _object->GetName())
				{
					if(target.Attach(dynamic_cast<QuaternionLerp*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg1.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG1])
		{
			if(_object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
			{
				if(templateArg1.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg1.GetResult() == _object->GetName())
				{
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg1.Attach(dynamic_cast<Variable<bool>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg1IsRenamed));
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
				if(arg1Name == _object->GetName())
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg1.Attach(dynamic_cast<Variable<bool>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg1IsRenamed));
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
			if(_object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
			{
				if(templateArg2.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg2.GetResult() == _object->GetName())
				{
					if(loadArgsEnable) { state[ASSET_TYPE][ARG2] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg2.Attach(dynamic_cast<Variable<bool>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG2])
		{
			if(_object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
			{
				if(arg2Name == _object->GetName())
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG2] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg2.Attach(dynamic_cast<Variable<bool>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg3.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG3])
		{
			if(_object->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS))
			{
				if(templateArg3.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg3.GetResult() == _object->GetName())
				{
					if(loadArgsEnable) { state[ASSET_TYPE][ARG3] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg3.Attach(dynamic_cast<RotatableObject*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG3])
		{
			if(_object->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS))
			{
				if(arg3Name == _object->GetName())
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG3] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg3.Attach(dynamic_cast<RotatableObject*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StreamingRotatableDeltaAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool StreamingRotatableDeltaAction::SaveToFile(StringANSI _path)			
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


bool StreamingRotatableDeltaAction::SaveToFile(File& _file)
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
		if(state[TEMPLATE][ARG2])
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
			_file.Write(GetBooleanKeyArg(ARG2));
		}
		if(state[TEMPLATE][ARG3])
		{
			_file.Write(GetTemplateArg(ARG3).length());
			_file.WriteString(GetTemplateArg(ARG3));
		}
		else if(state[VARIABLE][ARG3])
		{
			_file.Write(GetVariableArg(ARG3).length());
			_file.WriteString(GetVariableArg(ARG3));
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


AbstractAction* StreamingRotatableDeltaAction::_LoadFromFile(File& _file)
{
	StreamingRotatableDeltaAction* action = new StreamingRotatableDeltaAction(QuaternionLerp::STREAMING_ROTATABLE_DELTA_MESSAGE);
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
	if(action->state[TEMPLATE][ARG2])
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
		bool arg; 
		_file.Read(arg); 
		action->SetBooleanKeyArg(ARG2, arg);
	}
	if(action->state[TEMPLATE][ARG3])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(ARG3, templateArg); 
	}
	else if(action->state[VARIABLE][ARG3])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG3, variableArg);
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