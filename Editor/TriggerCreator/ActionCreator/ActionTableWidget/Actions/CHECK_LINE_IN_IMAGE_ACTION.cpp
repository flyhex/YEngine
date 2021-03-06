#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>



void ActionTableWidget::CHECK_LINE_IN_IMAGE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "CHECK_LINE_IN_IMAGE")
	{
		if(_index.column() == VALUE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
				    if(item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						_editor = UniversalDelegate::VECTOR_3_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::TRANSLATABLE_OBJECT_EDITOR;
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
				    if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						_editor = UniversalDelegate::VECTOR_3_EDITOR;
						return;
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::TRANSLATABLE_OBJECT_EDITOR;
						return;
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case ARG3_PROPERTY:
				{
				    if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::BOOLEAN_VARIABLE_EDITOR;
						return;
					}
					else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case ARG4_PROPERTY:
				{
				    if(item(ARG4_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::VECTOR_2_VARIABLE_EDITOR;
						return;
					}
					else if(item(ARG4_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case ARG5_PROPERTY:
				{
				    if(item(ARG5_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::COLORABLE_OBJECT_EDITOR;
						return;
					}
					else if(item(ARG5_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case TARGET_PROPERTY:
				{
					if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::OBJECT_EDITOR;
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

				case ARG4_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case ARG5_PROPERTY:
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




bool ActionTableWidget::CHECK_LINE_IN_IMAGE_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "CHECK_LINE_IN_IMAGE") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG1_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG3_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG3_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG4_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG4_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG5_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG5_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(TARGET_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(TARGET_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG1_PROPERTY);
			if(item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "Begin point"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "Vector3");
			}
			else
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "Begin point"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "Translatable object"); 
			}
		}
		else if(_index.row() == ARG2_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG2_PROPERTY);
			if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				SetCellText(ARG2_PROPERTY, CONTENT, "End point"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
			}
			else
			{
				SetCellText(ARG2_PROPERTY, CONTENT, "End point"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "Translatable object"); 
			}
		}
		else if(_index.row() == ARG3_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG3_PROPERTY);
			SetCellText(ARG3_PROPERTY, CONTENT, "Intersection result"); 
			SetCellText(ARG3_PROPERTY, DATA_TYPE, "Boolean variable"); 
		}
		else if(_index.row() == ARG4_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG4_PROPERTY);
			SetCellText(ARG4_PROPERTY, CONTENT, "Pixel position"); 
			SetCellText(ARG4_PROPERTY, DATA_TYPE, "Vector2 variable"); 
		}
		else if(_index.row() == ARG5_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG5_PROPERTY);
			SetCellText(ARG5_PROPERTY, CONTENT, "Pixel color"); 
			SetCellText(ARG5_PROPERTY, DATA_TYPE, "Colorable object"); 
		}
		else if(_index.row() == TARGET_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(TARGET_PROPERTY);
			SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
			SetCellText(TARGET_PROPERTY, DATA_TYPE, "Texturing surface"); 
		}
		return true;
	}  
	return false;
}



QString ActionTableWidget::CHECK_LINE_IN_IMAGE_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE")
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
		if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG2_PROPERTY, VALUE)->text();
		}
		if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG3_PROPERTY, VALUE)->text();
		}
		if(item(ARG4_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG4_PROPERTY, VALUE)->text();
		}
		if(item(ARG5_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG5_PROPERTY, VALUE)->text();
		}
		return name;
	}
	return "";
}


bool ActionTableWidget::CHECK_LINE_IN_IMAGE_UpdateTargetVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE")
	{ 
		UpdateObject(TARGET_PROPERTY);
		return true;
	}
	return false;
}



bool ActionTableWidget::CHECK_LINE_IN_IMAGE_UpdateArg1VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE")
	{
		UpdateTranslatableObject(ARG1_PROPERTY);
		return true;
	}
	return false;
}


bool ActionTableWidget::CHECK_LINE_IN_IMAGE_UpdateArg2VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE")
	{
		UpdateTranslatableObject(ARG2_PROPERTY);
		return true;
	}
	return false;
}


bool ActionTableWidget::CHECK_LINE_IN_IMAGE_UpdateArg3VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE")
	{
		UpdateVariable(ARG3_PROPERTY, Variable<bool>::BOOLEAN_TYPE); 
		return true;
	}
	return false;
}



bool ActionTableWidget::CHECK_LINE_IN_IMAGE_UpdateArg4VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE")
	{
		UpdateVariable(ARG4_PROPERTY, Variable<Vector2>::VECTOR_2_TYPE); 
		return true;
	}
	return false;
}


bool ActionTableWidget::CHECK_LINE_IN_IMAGE_UpdateArg5VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE")
	{
		UpdateColorableObject(ARG5_PROPERTY); 
		return true;
	}
	return false;
}

void ActionTableWidget::CHECK_LINE_IN_IMAGE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_LINE_IN_IMAGE")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG1_PROPERTY)
			{
				TranslatableObjectDropEvent(_it, _index, _suffix, _fileName);
			}
			else if(_index.row() == ARG2_PROPERTY)
			{
				TranslatableObjectDropEvent(_it, _index, _suffix, _fileName);
			}
			else if(_index.row() == ARG3_PROPERTY)
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<bool>::BOOLEAN_TYPE);
			}
			else if(_index.row() == ARG4_PROPERTY)
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<Vector2>::VECTOR_2_TYPE);
			}
			else if(_index.row() == ARG5_PROPERTY)
			{
				ColorableObjectDropEvent(_it, _index, _suffix, _fileName);
			}
			else if(_index.row() == TARGET_PROPERTY)
			{
				ObjectDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void ActionCreator::CHECK_LINE_IN_IMAGE_SetAction(int _actionType)
{
	if(_actionType == TexturingSurface::CHECK_LINE_IN_IMAGE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHECK_LINE_IN_IMAGE");

		CheckLineInImageAction* derived = dynamic_cast<CheckLineInImageAction*>(action);
		
		if(derived)
		{
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, Vector3ToString(derived->GetVector3KeyArg(AbstractAction::ARG1)));
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
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, Vector3ToString(derived->GetVector3KeyArg(AbstractAction::ARG2)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG2).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG2).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG3))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG3).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG3))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG3).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG4))
			{
				actionTable->SetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG4).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG4))
			{
				actionTable->SetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG4).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG5))
			{
				actionTable->SetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG5).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG5))
			{
				actionTable->SetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG5).c_str());
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


void ActionCreator::CHECK_LINE_IN_IMAGE_Create(void)
{
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "CHECK_LINE_IN_IMAGE")
	{
		action = Trigger::_CreateAction(TexturingSurface::CHECK_LINE_IN_IMAGE_MESSAGE);

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
				action->SetVector3KeyArg(AbstractAction::ARG1, StringToVector3(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE)));
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
			if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
			{
				action->SetVector3KeyArg(AbstractAction::ARG2, StringToVector3(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
			}
			else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
			{
				action->SetVariableArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG2) == AssetLibrary::UNKNOWN_ASSET)
				{
					int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

					action->SetArgAssetType(AbstractAction::ARG2, assetType);
				}
			}
			else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
			{
				action->SetTemplateArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
			}
			if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
			{
				action->SetVariableArg(AbstractAction::ARG3, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
				if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG3) == AssetLibrary::UNKNOWN_ASSET)
				{
					int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());

					action->SetArgAssetType(AbstractAction::ARG3, assetType);
				}
			}
			else if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
			{
				action->SetTemplateArg(AbstractAction::ARG3, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
			}
			if(actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
			{
				action->SetVariableArg(AbstractAction::ARG4, actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE).toStdString());
				if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG4) == AssetLibrary::UNKNOWN_ASSET)
				{
					int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE).toStdString());

					action->SetArgAssetType(AbstractAction::ARG4, assetType);
				}
			}
			else if(actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
			{
				action->SetTemplateArg(AbstractAction::ARG4, actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE).toStdString());
			}
			if(actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
			{
				action->SetVariableArg(AbstractAction::ARG5, actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE).toStdString());
				if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG5) == AssetLibrary::UNKNOWN_ASSET)
				{
					int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE).toStdString());

					action->SetArgAssetType(AbstractAction::ARG5, assetType);
				}
			}
			else if(actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
			{
				action->SetTemplateArg(AbstractAction::ARG5, actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE).toStdString());
			}
		}
	} 
}
