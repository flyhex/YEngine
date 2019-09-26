#include "TimerFinishEvent.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>




TimerFinishEvent::TimerFinishEvent(void)
{}



TimerFinishEvent::TimerFinishEvent(int32 _type): AbstractEvent(_type)
{}



TimerFinishEvent::~TimerFinishEvent(void)
{
	Destroying();
}


void TimerFinishEvent::UpdateValid(void)
{
	if(source.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
	}
}




void TimerFinishEvent::SetVariableArg(int32 _index, StringANSI _name)
{
	switch(_index)
	{
		case SOURCE:
		{
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateSource.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::TIMER_ASSET;

			SourceIsLost();
  
			sourceName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(sourceName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(sourceName);

			if(source.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Timer::TIMER_CLASS)) 
					{
						if(source.Attach(dynamic_cast<Timer*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &TimerFinishEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &TimerFinishEvent::SourceIsRenamed));
							object->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &TimerFinishEvent::EventHandler));
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


void TimerFinishEvent::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case SOURCE:
		{
			templateSource.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &TimerFinishEvent::TemplateSourceIsCompleted));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &TimerFinishEvent::SourceIsLost));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &TimerFinishEvent::TemplateSourceIsChanged));
			
			SetArgMode(_index, TEMPLATE); 
			state[ASSET_TYPE][_index] = AssetLibrary::TIMER_ASSET;

			SourceIsLost(); 

			if(templateSource.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateSourceIsCompleted();
			}
			break;
		}
	}
}



void TimerFinishEvent::TemplateSourceIsCompleted(void)
{
	if(templateSource.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateSource.GetResult(), state[ASSET_TYPE][SOURCE]) : AssetLibrary::_GetAssets().FindObject(templateSource.GetResult());

		if(source.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Timer::TIMER_CLASS)) 
				{
					if(source.Attach(dynamic_cast<Timer*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &TimerFinishEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &TimerFinishEvent::SourceIsRenamed));
						object->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &TimerFinishEvent::EventHandler));
						UpdateValid();
					}
				}
				return;
			}
		}
	}
}


void TimerFinishEvent::SourceIsLost(void)
{
	if(source.IsNotEmpty())
	{
		source.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		source.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		source.Get()->Disconnect(Timer::TIMER_FINISH_MESSAGE, this);
		source.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &TimerFinishEvent::ObjectIsCreated));
}



void TimerFinishEvent::TemplateSourceIsChanged(void)
{
	SourceIsLost();

	TemplateSourceIsCompleted(); 
}





StringANSI TimerFinishEvent::GetTemplateArg(int32 _index)
{
	switch(_index)
	{
		case SOURCE:
		{
			return templateSource.GetStringExpr();
		}
	}
	return "";
}



StringANSI TimerFinishEvent::GetVariableArg(int32 _index)
{
	switch(_index)
	{
		case SOURCE:
		{
			return sourceName;
		}
	}
	return "";
}


void TimerFinishEvent::SetValue(bool _value)
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


void TimerFinishEvent::EventHandler(void)
{
	SetValue(true); 
}


void TimerFinishEvent::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &TimerFinishEvent::ObjectIsCreated));
}


void TimerFinishEvent::SourceIsRenamed(AbstractObject::NameInfo* _info)
{
	SourceIsLost();
}
 


void TimerFinishEvent::ObjectIsCreated(AbstractObject* _object)
{
	if(source.IsEmpty())
	{
		if(state[TEMPLATE][SOURCE])
		{
			if(_object->IsClassPropertyExist(Timer::TIMER_CLASS)) 
			{
				if(templateSource.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateSource.GetResult() == _object->GetName())
				{
					if(source.Attach(dynamic_cast<Timer*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &TimerFinishEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &TimerFinishEvent::SourceIsRenamed));
						_object->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &TimerFinishEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][SOURCE])
		{
			if(_object->IsClassPropertyExist(Timer::TIMER_CLASS)) 
			{
				if(sourceName == _object->GetName())
				{
					if(source.Attach(dynamic_cast<Timer*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &TimerFinishEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &TimerFinishEvent::SourceIsRenamed));
						_object->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &TimerFinishEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool TimerFinishEvent::SaveToFile(StringANSI _path)			
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


bool TimerFinishEvent::SaveToFile(File& _file)
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


AbstractEvent* TimerFinishEvent::_LoadFromFile(File& _file)
{
	TimerFinishEvent* event = new TimerFinishEvent(Timer::TIMER_FINISH_MESSAGE);
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