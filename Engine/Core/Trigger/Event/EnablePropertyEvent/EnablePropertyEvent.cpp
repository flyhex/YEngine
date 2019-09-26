#include "EnablePropertyEvent.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>




EnablePropertyEvent::EnablePropertyEvent(void)
{}



EnablePropertyEvent::EnablePropertyEvent(int32 _type): AbstractEvent(_type)
{}



EnablePropertyEvent::~EnablePropertyEvent(void)
{
	Destroying();
}


void EnablePropertyEvent::UpdateValid(void)
{
	if(((arg1.IsNotEmpty() && arg2.IsNotEmpty()) || !argsEnable) && source.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
	}
}




void EnablePropertyEvent::SetVariableArg(int32 _index, StringANSI _name)
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
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg1IsRenamed));
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
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg2IsRenamed));
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

			SourceIsLost();
  
			sourceName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(sourceName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(sourceName);

			if(source.IsEmpty())
			{
				if(object)
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(source.Attach(object))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::SourceIsRenamed));
						object->Connect(AbstractObject::ENABLE_PROPERTY_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
			break;
		}
	}
}


void EnablePropertyEvent::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::TemplateArg1IsCompleted));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg1IsLost));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::TemplateArg1IsChanged));
			
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
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::TemplateArg2IsCompleted));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg2IsLost));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::TemplateArg2IsChanged));
			
			SetArgMode(_index, TEMPLATE);

			Arg2IsLost(); 

			if(templateArg2.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg2IsCompleted();
			}
			break;
		}  

		case SOURCE:
		{
			templateSource.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::TemplateSourceIsCompleted));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::SourceIsLost));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::TemplateSourceIsChanged));
			
			SetArgMode(_index, TEMPLATE);

			SourceIsLost(); 

			if(templateSource.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateSourceIsCompleted();
			}
			break;
		}
	}
}



void EnablePropertyEvent::SetIntegerKeyArg(int32 _index, int32 _value)
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



void EnablePropertyEvent::SetBooleanKeyArg(int32 _index, bool _value)
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



void EnablePropertyEvent::TemplateArg1IsCompleted(void)
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
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void EnablePropertyEvent::Arg1IsLost(void)
{
	if(arg1.IsNotEmpty())
	{
		arg1.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg1.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg1.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsCreated));
}



void EnablePropertyEvent::TemplateArg1IsChanged(void)
{
	Arg1IsLost();

	TemplateArg1IsCompleted(); 
}



void EnablePropertyEvent::TemplateArg2IsCompleted(void)
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
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void EnablePropertyEvent::Arg2IsLost(void)
{
	if(arg2.IsNotEmpty())
	{
		arg2.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg2.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg2.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsCreated));
}



void EnablePropertyEvent::TemplateArg2IsChanged(void)
{
	Arg2IsLost();

	TemplateArg2IsCompleted(); 
}



void EnablePropertyEvent::TemplateSourceIsCompleted(void)
{
	if(templateSource.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateSource.GetResult(), state[ASSET_TYPE][SOURCE]) : AssetLibrary::_GetAssets().FindObject(templateSource.GetResult());

		if(source.IsEmpty())
		{
			if(object)
			{
				if(loadArgsEnable) { state[ASSET_TYPE][SOURCE] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
				if(source.Attach(object))
				{
					object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
					object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::SourceIsRenamed));
					object->Connect(AbstractObject::ENABLE_PROPERTY_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::EventHandler));
					UpdateValid();
				}
				return;
			}
		}
	}
}


void EnablePropertyEvent::SourceIsLost(void)
{
	if(source.IsNotEmpty())
	{
		source.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		source.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		source.Get()->Disconnect(AbstractObject::ENABLE_PROPERTY_MESSAGE, this);
		source.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsCreated));
}



void EnablePropertyEvent::TemplateSourceIsChanged(void)
{
	SourceIsLost();

	TemplateSourceIsCompleted(); 
}





StringANSI EnablePropertyEvent::GetTemplateArg(int32 _index)
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

		case SOURCE:
		{
			return templateSource.GetStringExpr();
		}
	}
	return "";
}



int32 EnablePropertyEvent::GetIntegerKeyArg(int32 _index)
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
	return -1;
}



bool EnablePropertyEvent::GetBooleanKeyArg(int32 _index)
{
	switch(_index)
	{
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


StringANSI EnablePropertyEvent::GetVariableArg(int32 _index)
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

		case SOURCE:
		{
			return sourceName;
		}
	}
	return "";
}


void EnablePropertyEvent::SetValue(bool _value)
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


void EnablePropertyEvent::EventHandler(AbstractObject::PropertyInfo* _info)
{
	if(argsEnable)
	{
		if(arg1.IsNotEmpty())
		{ 
			if(arg1.Get()->GetValue() == _info->property)
			{ 
				if(arg2.IsNotEmpty())
				{ 
					if(arg2.Get()->GetValue() == _info->enable)
					{
						SetValue(true);
					}
					else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
					{
						SetValue(false);
					} 
				}
				else
				{
					SetValue(true);
				}
			}
			else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
			{
				SetValue(false);
			} 
		}
		else if(arg2.IsNotEmpty())
		{ 
			if(arg2.Get()->GetValue() == _info->enable)
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
		SetValue(true);
	}
}


void EnablePropertyEvent::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsCreated));
}


void EnablePropertyEvent::SourceIsRenamed(AbstractObject::NameInfo* _info)
{
	SourceIsLost();
}
 


void EnablePropertyEvent::Arg1IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg1IsLost();
}



void EnablePropertyEvent::Arg2IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg2IsLost();
}


void EnablePropertyEvent::ObjectIsCreated(AbstractObject* _object)
{
	if(source.IsEmpty())
	{
		if(state[TEMPLATE][SOURCE])
		{
			if(templateSource.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateSource.GetResult() == _object->GetName())
			{ 
				if(loadArgsEnable) { state[ASSET_TYPE][SOURCE] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
				if(source.Attach(_object))
				{
					_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
					_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::SourceIsRenamed));
					_object->Connect(AbstractObject::ENABLE_PROPERTY_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::EventHandler));
					UpdateValid();
				}
				return;
			}
		}
		else if(state[VARIABLE][SOURCE])
		{
			if(sourceName == _object->GetName())
			{
				if(loadArgsEnable) { state[ASSET_TYPE][SOURCE] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
				if(source.Attach(_object))
				{
					_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
					_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::SourceIsRenamed));
					_object->Connect(AbstractObject::ENABLE_PROPERTY_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::EventHandler));
					UpdateValid();
				}
				return;
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
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg1IsRenamed));
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
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg1IsRenamed));
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
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg2IsRenamed));
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
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &EnablePropertyEvent::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool EnablePropertyEvent::SaveToFile(StringANSI _path)			
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


bool EnablePropertyEvent::SaveToFile(File& _file)
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
			_file.Write(GetBooleanKeyArg(ARG2));
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


AbstractEvent* EnablePropertyEvent::_LoadFromFile(File& _file)
{
	EnablePropertyEvent* event = new EnablePropertyEvent(AbstractObject::ENABLE_PROPERTY_MESSAGE);
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
	if(event->state[TEMPLATE][ARG1])
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
		int32 arg; 
		_file.Read(arg); 
		event->SetIntegerKeyArg(ARG1, arg);
	}
	if(event->state[TEMPLATE][ARG2])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(ARG2, templateArg); 
	}
	else if(event->state[VARIABLE][ARG2])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(ARG2, variableArg);
	}
	else if(event->state[KEY][ARG2])
	{
		bool arg; 
		_file.Read(arg); 
		event->SetBooleanKeyArg(ARG2, arg);
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