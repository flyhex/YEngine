#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>



void ActionTableWidget::CLEAR_INPUT_EVENTS_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "CLEAR_INPUT_EVENTS")
	{
		if(_index.column() == VALUE)
		{
			switch(_index.row())
			{
				case TARGET_PROPERTY:
				{
					if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::PLAYER_CONTROL_EDITOR;
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




bool ActionTableWidget::CLEAR_INPUT_EVENTS_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "CLEAR_INPUT_EVENTS") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			SetPropertyEnable(ARG1_PROPERTY, false);
			SetPropertyEnable(ARG2_PROPERTY, false);
			SetPropertyEnable(ARG3_PROPERTY, false);
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(TARGET_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(TARGET_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
		}
		else if(_index.row() == TARGET_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(TARGET_PROPERTY);
			SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
			SetCellText(TARGET_PROPERTY, DATA_TYPE, "Player control"); 
		}
		return true;
	}  
	return false;
}



QString ActionTableWidget::CLEAR_INPUT_EVENTS_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CLEAR_INPUT_EVENTS")
	{ 
		QString name;

		if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(TARGET_PROPERTY, VALUE)->text(); 
		}
		return name;
	}
	return "";
}


bool ActionTableWidget::CLEAR_INPUT_EVENTS_UpdateTargetVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CLEAR_INPUT_EVENTS")
	{ 
		UpdatePlayerControl(TARGET_PROPERTY);
		return true;
	}
	return false;
}



void ActionTableWidget::CLEAR_INPUT_EVENTS_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CLEAR_INPUT_EVENTS")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == TARGET_PROPERTY)
			{
				PlayerControlDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void ActionCreator::CLEAR_INPUT_EVENTS_SetAction(int _actionType)
{
	if(_actionType == PlayerControl::CLEAR_INPUT_EVENTS_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CLEAR_INPUT_EVENTS");

		ClearInputEventsAction* derived = dynamic_cast<ClearInputEventsAction*>(action);
		
		if(derived)
		{
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


void ActionCreator::CLEAR_INPUT_EVENTS_Create(void)
{
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "CLEAR_INPUT_EVENTS")
	{
		action = Trigger::_CreateAction(PlayerControl::CLEAR_INPUT_EVENTS_MESSAGE);

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
		}
	} 
}
