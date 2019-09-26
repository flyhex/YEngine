#include "ChangeRotationRangeAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>


ChangeRotationRangeAction::ChangeRotationRangeAction(void)
{}


ChangeRotationRangeAction::ChangeRotationRangeAction(int32 _type): AbstractAction(_type)
{}



ChangeRotationRangeAction::~ChangeRotationRangeAction(void)
{
	Destroying();
}


void ChangeRotationRangeAction::UpdateValid(void)
{
	valid = false; 

	if(arg1.IsNotEmpty() && arg2.IsNotEmpty() && arg3.IsNotEmpty() && target.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
		valid = true;
	}
}


void ChangeRotationRangeAction::SetVariableArg(int32 _index, StringANSI _name)
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
					if(object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS)) 
					{
						if(arg1.Attach(dynamic_cast<Variable<int32>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg1IsRenamed));
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
					if(object->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS)) 
					{
						if(arg2.Attach(dynamic_cast<Variable<float>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg2IsRenamed));
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
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg3IsLost(); 
			
			arg3Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg3Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg3Name);

			if(arg3.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS)) 
					{
						if(arg3.Attach(dynamic_cast<Variable<float>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg3IsRenamed));
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
			state[ASSET_TYPE][_index] = AssetLibrary::CAMERA_ASSET;

			TargetIsLost();
  
			targetName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(targetName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(targetName);

			if(target.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Camera3D::CAMERA_3D_CLASS))
					{
						if(target.Attach(dynamic_cast<Camera3D*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TargetIsRenamed));
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


void ChangeRotationRangeAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TemplateArg1IsCompleted));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg1IsLost));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TemplateArg1IsChanged));
			 
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
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TemplateArg2IsCompleted));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg2IsLost));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TemplateArg2IsChanged));
			
			SetArgMode(_index, TEMPLATE); 
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg2IsLost(); 

			if(templateArg2.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg2IsCompleted();
			}
			break;
		}

		case ARG3:
		{
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TemplateArg3IsCompleted));
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg3IsLost));
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TemplateArg3IsChanged));
			
			SetArgMode(_index, TEMPLATE); 
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg3IsLost(); 

			if(templateArg3.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg3IsCompleted();
			}
			break;
		}

		case TARGET:
		{
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TemplateTargetIsCompleted));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TargetIsLost));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TemplateTargetIsChanged));
			
			SetArgMode(_index, TEMPLATE); 
			state[ASSET_TYPE][_index] = AssetLibrary::CAMERA_ASSET;

			TargetIsLost(); 

			if(templateTarget.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateTargetIsCompleted();
			}
			break;
		}
	}
}


void ChangeRotationRangeAction::SetIntegerKeyArg(int32 _index, int32 _value)
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
			Variable<int32>* object = new Variable<int32>();
			object->SetValue(_value);
			
			if(arg1.Attach(object))
			{
				UpdateValid();
			}
			break;
		}
	}
}



void ChangeRotationRangeAction::SetFloatKeyArg(int32 _index, float _value)
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
			Variable<float>* object = new Variable<float>();
			object->SetValue(_value);
			
			if(arg2.Attach(object))
			{
				UpdateValid();
			}
			break;
		}

		case ARG3:
		{
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg3.Destroy();
			
			SetArgMode(_index, KEY);

			Arg3IsLost();
			Variable<float>* object = new Variable<float>();
			object->SetValue(_value);
			
			if(arg3.Attach(object))
			{
				UpdateValid();
			}
			break;
		}
	}
}


void ChangeRotationRangeAction::TemplateArg1IsCompleted(void)
{
	if(templateArg1.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg1.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg1.GetResult());

		if(arg1.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS)) 
				{
					if(arg1.Attach(dynamic_cast<Variable<int32>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void ChangeRotationRangeAction::Arg1IsLost(void)
{
	if(arg1.IsNotEmpty())
	{ 
		arg1.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg1.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg1.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsCreated));
	valid = false;
}



void ChangeRotationRangeAction::TemplateArg1IsChanged(void)
{
	Arg1IsLost();

	TemplateArg1IsCompleted(); 
}



void ChangeRotationRangeAction::TemplateArg2IsCompleted(void)
{
	if(templateArg2.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg2.GetResult(), state[ASSET_TYPE][ARG2]) : AssetLibrary::_GetAssets().FindObject(templateArg2.GetResult());

		if(arg2.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
				{
					if(arg2.Attach(dynamic_cast<Variable<float>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void ChangeRotationRangeAction::Arg2IsLost(void)
{
	if(arg2.IsNotEmpty())
	{ 
		arg2.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg2.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg2.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsCreated));
	valid = false;
}


void ChangeRotationRangeAction::TemplateArg2IsChanged(void)
{
	Arg2IsLost();

	TemplateArg2IsCompleted(); 
}



void ChangeRotationRangeAction::TemplateArg3IsCompleted(void)
{
	if(templateArg3.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg3.GetResult(), state[ASSET_TYPE][ARG3]) : AssetLibrary::_GetAssets().FindObject(templateArg3.GetResult());

		if(arg3.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
				{
					if(arg3.Attach(dynamic_cast<Variable<float>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void ChangeRotationRangeAction::Arg3IsLost(void)
{
	if(arg3.IsNotEmpty())
	{ 
		arg3.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg3.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg3.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsCreated));
	valid = false;
}


void ChangeRotationRangeAction::TemplateArg3IsChanged(void)
{
	Arg3IsLost();

	TemplateArg3IsCompleted(); 
}



void ChangeRotationRangeAction::TemplateTargetIsCompleted(void)
{
	if(templateTarget.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateTarget.GetResult(), state[ASSET_TYPE][TARGET]) : AssetLibrary::_GetAssets().FindObject(templateTarget.GetResult());

		if(target.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Camera3D::CAMERA_3D_CLASS)) 
				{
					if(target.Attach(dynamic_cast<Camera3D*>(object)))
					{ 
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void ChangeRotationRangeAction::TargetIsLost(void)
{
	if(target.IsNotEmpty())
	{
		target.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		target.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		target.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsCreated));
	valid = false;
}



void ChangeRotationRangeAction::TemplateTargetIsChanged(void)
{
	TargetIsLost();

	TemplateTargetIsCompleted(); 
}



StringANSI ChangeRotationRangeAction::GetTemplateArg(int32 _index)
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



StringANSI ChangeRotationRangeAction::GetVariableArg(int32 _index)
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



int32 ChangeRotationRangeAction::GetIntegerKeyArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			if(arg1.IsNotEmpty())
			{
				return arg1.Get()->GetValue();
			}
			break;
		}
	}
	return -1;
}



float ChangeRotationRangeAction::GetFloatKeyArg(int32 _index)
{
	switch(_index)
	{
		case ARG2:
		{
			if(arg2.IsNotEmpty())
			{
				return arg2.Get()->GetValue();
			}
			break;
		}
		
		case ARG3:
		{
			if(arg3.IsNotEmpty())
			{
				return arg3.Get()->GetValue();
			}
			break;
		}
	}
	return -1.0f;
}



void ChangeRotationRangeAction::operator () (void)
{
	if(IsValid()) 
	{ 
		switch(arg1.Get()->GetValue())
		{
			case Vector3::X:
			{
				target.Get()->SetPitchMin(arg2.Get()->GetValue());
				target.Get()->SetPitchMax(arg3.Get()->GetValue());
				break;
			}

			case Vector3::Y:
			{
				target.Get()->SetYawMin(arg2.Get()->GetValue());
				target.Get()->SetYawMax(arg3.Get()->GetValue());
				break;
			}

			case Vector3::Z:
			{
				target.Get()->SetRollMin(arg2.Get()->GetValue());
				target.Get()->SetRollMax(arg3.Get()->GetValue());
				break;
			}
		}
	}
}



void ChangeRotationRangeAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsCreated));
	valid = false;
}



void ChangeRotationRangeAction::TargetIsRenamed(AbstractObject::NameInfo* _info)
{
	TargetIsLost();
}

 

void ChangeRotationRangeAction::Arg1IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg1IsLost();
}


void ChangeRotationRangeAction::Arg2IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg2IsLost();
}


void ChangeRotationRangeAction::Arg3IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg3IsLost();
}


void ChangeRotationRangeAction::ObjectIsCreated(AbstractObject* _object)
{
	if(target.IsEmpty())
	{
		if(state[TEMPLATE][TARGET])
		{
			if(_object->IsClassPropertyExist(Camera3D::CAMERA_3D_CLASS))
			{
				if(templateTarget.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateTarget.GetResult() == _object->GetName())
				{
					if(target.Attach(dynamic_cast<Camera3D*>(_object)))
					{ 
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][TARGET])
		{
			if(_object->IsClassPropertyExist(Camera3D::CAMERA_3D_CLASS))
			{
				if(targetName == _object->GetName())
				{
					if(target.Attach(dynamic_cast<Camera3D*>(_object)))
					{ 
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::TargetIsRenamed));
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
			if(_object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
			{
				if(templateArg1.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg1.GetResult() == _object->GetName())
				{
					if(arg1.Attach(dynamic_cast<Variable<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG1])
		{
			if(_object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
			{
				if(arg1Name == _object->GetName())
				{
					if(arg1.Attach(dynamic_cast<Variable<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg1IsRenamed));
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
			if(_object->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
			{
				if(templateArg2.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg2.GetResult() == _object->GetName())
				{
					if(arg2.Attach(dynamic_cast<Variable<float>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG2])
		{
			if(_object->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
			{
				if(arg2Name == _object->GetName())
				{
					if(arg2.Attach(dynamic_cast<Variable<float>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg2IsRenamed));
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
			if(_object->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
			{
				if(templateArg3.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg3.GetResult() == _object->GetName())
				{
					if(arg3.Attach(dynamic_cast<Variable<float>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG3])
		{
			if(_object->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
			{
				if(arg3Name == _object->GetName())
				{
					if(arg3.Attach(dynamic_cast<Variable<float>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeRotationRangeAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool ChangeRotationRangeAction::SaveToFile(StringANSI _path)			
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


bool ChangeRotationRangeAction::SaveToFile(File& _file)
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
			_file.Write(GetIntegerKeyArg(ARG1));
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
			_file.Write(GetFloatKeyArg(ARG2));
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
		else if(state[KEY][ARG3])
		{
			_file.Write(GetFloatKeyArg(ARG3));
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


AbstractAction* ChangeRotationRangeAction::_LoadFromFile(File& _file)
{
	ChangeRotationRangeAction* action = new ChangeRotationRangeAction(Camera3D::CHANGE_ROTATION_RANGE_MESSAGE);
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
		int32 arg; 
		_file.Read(arg); 
		action->SetIntegerKeyArg(ARG1, arg);
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
		float arg; 
		_file.Read(arg); 
		action->SetFloatKeyArg(ARG2, arg);
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
	else if(action->state[KEY][ARG3])
	{
		float arg; 
		_file.Read(arg); 
		action->SetFloatKeyArg(ARG3, arg);
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