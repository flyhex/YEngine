#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>



void ActionTableWidget::STREAMING_QUATERNION_CURRENT_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "STREAMING_QUATERNION_CURRENT")
	{
		if(_index.column() == VALUE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
				    if(item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						_editor = UniversalDelegate::BOOL_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::BOOLEAN_VARIABLE_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case ARG2_PROPERTY:
				{
				    _editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case ARG3_PROPERTY:
				{
					if(item(ARG2_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
					{
						if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::ROTATABLE_OBJECT_EDITOR;
							return;
						}
						else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					else if(item(ARG2_PROPERTY, VALUE)->text() == "QUATERNION")
					{
						if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::QUATERNION_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					break;
				}
				
				case TARGET_PROPERTY:
				{
					if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::QUATERNION_LERP_EDITOR;
						return;
					}
					else if(item(TARGET_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}
			}
		}
		else if(_index.column() == ARG_MODE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case ARG2_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case ARG3_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case TARGET_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}
			}
		}
		_editor = UniversalDelegate::NO_EDIT;
	}
}




bool ActionTableWidget::STREAMING_QUATERNION_CURRENT_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "STREAMING_QUATERNION_CURRENT") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG1_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, VALUE)), quaternionLerpArgsComboList);

			SetCellText(ARG2_PROPERTY, CONTENT, "Arg type");
			delegate->SetComboValuesForCell(indexFromItem(item(ARG3_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG3_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(TARGET_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(TARGET_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG1_PROPERTY);
			if(item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "Enable"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "Boolean");
			}
			else
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "Enable"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "Boolean variable"); 
			}
		} 
		else if((_index.row() == ARG2_PROPERTY && _index.column() == VALUE) ||
			    (_index.row() == ARG3_PROPERTY && _index.column() == ARG_MODE))
		{
			SetDefaultProperty(ARG3_PROPERTY);

			if(item(ARG2_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
			{
				SetCellText(ARG3_PROPERTY, CONTENT, "Result"); 
				SetCellText(ARG3_PROPERTY, DATA_TYPE, "Rotatable object"); 
			}
			else if(item(ARG2_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				SetCellText(ARG3_PROPERTY, CONTENT, "Result"); 
				SetCellText(ARG3_PROPERTY, DATA_TYPE, "Quaternion variable"); 
			}
		}
		else if(_index.row() == TARGET_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(TARGET_PROPERTY);
			SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
			SetCellText(TARGET_PROPERTY, DATA_TYPE, "Quaternion lerp"); 
		}
		return true;
	}  
	return false;
}



QString ActionTableWidget::STREAMING_QUATERNION_CURRENT_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "STREAMING_QUATERNION_CURRENT")
	{ 
		QString name;

		if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(TARGET_PROPERTY, VALUE)->text(); 
		}
		if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG1_PROPERTY, VALUE)->text();
		}
		if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG3_PROPERTY, VALUE)->text();
		}
		return name;
	}
	return "";
}


bool ActionTableWidget::STREAMING_QUATERNION_CURRENT_UpdateTargetVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "STREAMING_QUATERNION_CURRENT")
	{ 
		UpdateQuaternionLerp(TARGET_PROPERTY);
		return true;
	}
	return false;
}



bool ActionTableWidget::STREAMING_QUATERNION_CURRENT_UpdateArg1VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "STREAMING_QUATERNION_CURRENT")
	{
		UpdateVariable(ARG1_PROPERTY, Variable<bool>::BOOLEAN_TYPE);
		return true;
	}
	return false;
}


bool ActionTableWidget::STREAMING_QUATERNION_CURRENT_UpdateArg2PropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "STREAMING_QUATERNION_CURRENT")
	{ 
		SetPropertyValid(ARG2_PROPERTY, true);
		return true;
	}
	return false;
}


bool ActionTableWidget::STREAMING_QUATERNION_CURRENT_UpdateArg3VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "STREAMING_QUATERNION_CURRENT")
	{
		if(item(ARG2_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
		{
			UpdateRotatableObject(ARG3_PROPERTY);
		}
		else if(item(ARG2_PROPERTY, VALUE)->text() == "QUATERNION")
		{
			UpdateVariable(ARG3_PROPERTY, Variable<Quaternion>::QUATERNION_TYPE);
		}
		return true;
	}
	return false;
}


void ActionTableWidget::STREAMING_QUATERNION_CURRENT_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "STREAMING_QUATERNION_CURRENT")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG1_PROPERTY)
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<bool>::BOOLEAN_TYPE);
			}
			else if(_index.row() == ARG3_PROPERTY)
			{
				if(item(ARG2_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
				{
					RotatableObjectDropEvent(_it, _index, _suffix, _fileName);
				}
				else if(item(ARG2_PROPERTY, VALUE)->text() == "QUATERNION")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<Quaternion>::QUATERNION_TYPE);
				}
			}
			else if(_index.row() == TARGET_PROPERTY)
			{
				QuaternionLerpDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void ActionCreator::STREAMING_QUATERNION_CURRENT_SetAction(int _actionType)
{
	if(_actionType == QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "STREAMING_QUATERNION_CURRENT");

		StreamingQuaternionCurrentAction* derived = dynamic_cast<StreamingQuaternionCurrentAction*>(action);
		
		if(derived)
		{
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetBooleanKeyArg(AbstractAction::ARG1) ? "true" : "false");
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
			}
			actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, "QUATERNION");
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG2).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG2).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::TARGET))
			{
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::TARGET).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::TARGET))
			{
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::TARGET).c_str());
			}
		}
	}
	else if(_actionType == QuaternionLerp::STREAMING_ROTATABLE_CURRENT_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "STREAMING_QUATERNION_CURRENT");

		StreamingRotatableCurrentAction* derived = dynamic_cast<StreamingRotatableCurrentAction*>(action);
		
		if(derived)
		{
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetBooleanKeyArg(AbstractAction::ARG1) ? "true" : "false");
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
			}
			actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, "ROTATABLE_OBJECT");
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG2).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG2).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::TARGET))
			{
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::TARGET).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::TARGET))
			{
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::TARGET).c_str());
			}
		}
	}
}


void ActionCreator::STREAMING_QUATERNION_CURRENT_Create(void)
{
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "STREAMING_QUATERNION_CURRENT")
	{
		if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE) == "QUATERNION")
		{
			action = Trigger::_CreateAction(QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE);

			if(action)
			{
				if(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::TARGET, actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::TARGET) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::TARGET, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::TARGET, actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetBooleanKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "true" ? true : false);
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG2) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG2, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE) == "ROTATABLE_OBJECT")
		{  
			action = Trigger::_CreateAction(QuaternionLerp::STREAMING_ROTATABLE_CURRENT_MESSAGE);

			if(action)
			{
				if(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::TARGET, actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::TARGET) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::TARGET, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::TARGET, actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetBooleanKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "true" ? true : false);
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG2) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG2, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
	} 
}
