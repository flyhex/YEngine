#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>


void ActionTableWidget::REMOVE_FROM_LAYER_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "REMOVE_FROM_LAYER")
	{
		if(_index.column() == VALUE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
				    if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::ANY_ASSET_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
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
						_editor = UniversalDelegate::SCENE_EDITOR;
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




bool ActionTableWidget::REMOVE_FROM_LAYER_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "REMOVE_FROM_LAYER") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			SetPropertyEnable(ARG2_PROPERTY, false);
			SetPropertyEnable(ARG3_PROPERTY, false);
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG1_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(TARGET_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(TARGET_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG1_PROPERTY);
			SetCellText(ARG1_PROPERTY, CONTENT, "Name"); 
			SetCellText(ARG1_PROPERTY, DATA_TYPE, "Abstract object"); 
		}
		else if(_index.row() == TARGET_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(TARGET_PROPERTY);
			SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
			SetCellText(TARGET_PROPERTY, DATA_TYPE, "Layer"); 
		}
		return true;
	}  
	return false;
}



QString ActionTableWidget::REMOVE_FROM_LAYER_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "REMOVE_FROM_LAYER")
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
		return name;
	}
	return "";
}


bool ActionTableWidget::REMOVE_FROM_LAYER_UpdateTargetVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "REMOVE_FROM_LAYER")
	{ 
		UpdateScene(TARGET_PROPERTY);
		return true;
	}
	return false;
}



bool ActionTableWidget::REMOVE_FROM_LAYER_UpdateArg1VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "REMOVE_FROM_LAYER")
	{
		UpdateAnyAsset(ARG1_PROPERTY); 
		return true;
	}
	return false;
}


void ActionTableWidget::REMOVE_FROM_LAYER_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "REMOVE_FROM_LAYER")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG1_PROPERTY)
			{
				AnyAssetDropEvent(_it, _index, _suffix, _fileName);
			}
			else if(_index.row() == TARGET_PROPERTY)
			{
				SceneDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void ActionCreator::REMOVE_FROM_LAYER_SetAction(int _actionType)
{
	if(_actionType == ObjectScene::REMOVE_FROM_LAYER_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "REMOVE_FROM_LAYER");

		RemoveFromLayerAction* derived = dynamic_cast<RemoveFromLayerAction*>(action);
		
		if(derived)
		{
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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


void ActionCreator::REMOVE_FROM_LAYER_Create(void)
{
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "REMOVE_FROM_LAYER")
	{
		action = Trigger::_CreateAction(ObjectScene::REMOVE_FROM_LAYER_MESSAGE);

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
			if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
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
		}
	} 
}
