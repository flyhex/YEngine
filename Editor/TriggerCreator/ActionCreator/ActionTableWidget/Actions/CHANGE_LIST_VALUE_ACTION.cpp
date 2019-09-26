#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>



void ActionTableWidget::CHANGE_LIST_VALUE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "CHANGE_LIST_VALUE")
	{
		if(_index.column() == VALUE)
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
					if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
						{
							_editor = UniversalDelegate::INT_EX_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
						{
							_editor = UniversalDelegate::FLOAT_EX_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
						{
							_editor = UniversalDelegate::VECTOR_2_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
						   	    item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT" ||
								item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT" ||
								item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
						{
							_editor = UniversalDelegate::VECTOR_3_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION" ||
							    item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
						{
							_editor = UniversalDelegate::QUATERNION_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOL_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{ 
							_editor = UniversalDelegate::TEXT_EDITOR;
							return;
						}
					}
				    else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
						{
							_editor = UniversalDelegate::INTEGER_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
						{
							_editor = UniversalDelegate::FLOAT_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
						{
							_editor = UniversalDelegate::VECTOR_2_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
						{
							_editor = UniversalDelegate::VECTOR_3_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
						{
							_editor = UniversalDelegate::QUATERNION_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOLEAN_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{
							_editor = UniversalDelegate::STRING_VARIABLE_EDITOR;
							return;
						} 
						else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
						{
							_editor = UniversalDelegate::TRANSLATABLE_OBJECT_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
						{
							_editor = UniversalDelegate::SCALABLE_OBJECT_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
						{
							_editor = UniversalDelegate::ROTATABLE_OBJECT_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
						{
							_editor = UniversalDelegate::COLORABLE_OBJECT_EDITOR;
							return;
						}
					}  
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
					{
						if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
						{
							_editor = UniversalDelegate::INTEGER_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
						{
							_editor = UniversalDelegate::FLOAT_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2") 
						{
							_editor = UniversalDelegate::VECTOR_2_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
							    item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT" ||
								item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT" ||
								item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
						{
							_editor = UniversalDelegate::VECTOR_3_ARITHMETIC_EXPR_EDITOR;
							return;
						} 
						else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION" ||
							    item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
						{ 
							_editor = UniversalDelegate::QUATERNION_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOLEAN_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
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
						if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
						{
							_editor = UniversalDelegate::INTEGER_LIST_EDITOR;
							return;
						}
					    else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
						{
							_editor = UniversalDelegate::FLOAT_LIST_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
						{
							_editor = UniversalDelegate::VECTOR_2_LIST_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
						{
							_editor = UniversalDelegate::VECTOR_3_LIST_EDITOR;
							return; 
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
						{
							_editor = UniversalDelegate::QUATERNION_LIST_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{
							_editor = UniversalDelegate::STRING_LIST_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOLEAN_LIST_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
						{
							_editor = UniversalDelegate::TRANSLATABLE_LIST_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
						{
							_editor = UniversalDelegate::SCALABLE_LIST_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
						{
							_editor = UniversalDelegate::ROTATABLE_LIST_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
						{
							_editor = UniversalDelegate::COLORABLE_LIST_EDITOR;
							return;
						}
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
				case ARG2_PROPERTY:
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




bool ActionTableWidget::CHANGE_LIST_VALUE_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "CHANGE_LIST_VALUE") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			SetPropertyEnable(ARG3_PROPERTY, false);
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, VALUE)), createListArgsComboList);

			SetCellText(ARG1_PROPERTY, CONTENT, "List type");
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList);
			SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(TARGET_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(TARGET_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == VALUE)
		{ 
			SetDefaultProperty(ARG2_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{ 
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector2");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector2 variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3 variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{ 
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "String expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Boolean expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Translatable object"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Scalable object"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
			{ 
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Rotatable object"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Color RGB");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Colorable object"); 
				}
			}
			
			SetDefaultProperty(TARGET_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Integer list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Float list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Vector2 list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Vector3 list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Quaternion list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "String list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Boolean list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Translatable list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Scalable list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Rotatable list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Colorable list");
			}
		}
		else if(_index.row() == ARG2_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG2_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{ 
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector2");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector2 variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3 variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "String expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Boolean expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Translatable object"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Scalable object"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Rotatable object"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Color RGB");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Colorable object"); 
				}
			} 
		}
		else if(_index.row() == TARGET_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(TARGET_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Integer list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Float list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Vector2 list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Vector3 list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Quaternion list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "String list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Boolean list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Translatable list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Scalable list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Rotatable list");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Colorable list");
			}
		}
		return true;
	}  
	return false;
}
 


QString ActionTableWidget::CHANGE_LIST_VALUE_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_LIST_VALUE")
	{ 
		QString name;

		if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(TARGET_PROPERTY, VALUE)->text(); 
		}
		if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG2_PROPERTY, VALUE)->text();
		}
		return name;
	}
	return "";
}



bool ActionTableWidget::CHANGE_LIST_VALUE_UpdateTargetVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_LIST_VALUE")
	{ 
		UpdateList(TARGET_PROPERTY, GetListType(item(ARG1_PROPERTY, VALUE)->text()));
		return true; 
	}
	return false;
}



bool ActionTableWidget::CHANGE_LIST_VALUE_UpdateArg1PropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_LIST_VALUE")
	{ 
		SetPropertyValid(ARG1_PROPERTY, true); 
		if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
		{
			SetPropertyValid(ARG2_PROPERTY, true);
		}
		return true;
	}
	return false;
}


bool ActionTableWidget::CHANGE_LIST_VALUE_UpdateArg2VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_LIST_VALUE")
	{
		if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "STRING" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
		{
			UpdateVariable(ARG2_PROPERTY, GetVariableType(item(ARG1_PROPERTY, VALUE)->text()));
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
		{ 
			UpdateScalableObject(ARG2_PROPERTY);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
		{
			UpdateTranslatableObject(ARG2_PROPERTY);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
		{
			UpdateRotatableObject(ARG2_PROPERTY);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
		{
			UpdateColorableObject(ARG2_PROPERTY);
		}  
		return true;
	}
	return false;
}


 
bool ActionTableWidget::CHANGE_LIST_VALUE_UpdateArg2ExpressionPropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_LIST_VALUE")
	{ 
		if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
		{
			UpdateArithmeticExpr(ARG2_PROPERTY, GetVariableType(item(ARG1_PROPERTY, VALUE)->text()));
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT" ||
			    item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT" ||
				item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
		{
			UpdateArithmeticExpr(ARG2_PROPERTY, Variable<Vector3>::VECTOR_3_TYPE);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
		{
			UpdateArithmeticExpr(ARG2_PROPERTY, Variable<Quaternion>::QUATERNION_TYPE);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
		{ 
			UpdateStringExpr(ARG2_PROPERTY);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
		{
			UpdateBooleanExpr(ARG2_PROPERTY);
		}
		return true;
	}
	return false;
}

 

void ActionTableWidget::CHANGE_LIST_VALUE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_LIST_VALUE")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG2_PROPERTY)
			{ 
				if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "STRING" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, GetVariableType(item(ARG1_PROPERTY, VALUE)->text()));
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
				{
					TranslatableObjectDropEvent(_it, _index, _suffix, _fileName);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
				{
					ColorableObjectDropEvent(_it, _index, _suffix, _fileName);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
				{
					ScalableObjectDropEvent(_it, _index, _suffix, _fileName);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
				{
					RotatableObjectDropEvent(_it, _index, _suffix, _fileName);
				}
			}
			else if(_index.row() == TARGET_PROPERTY)
			{
				if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "STRING" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
				{
					ListDropEvent(_it, _index, _suffix, _fileName, GetListType(item(ARG1_PROPERTY, VALUE)->text()));
				}
			}
		}   
		else if(item(_index.row(), ARG_MODE)->text() == "EXPRESSION")
		{
			if(_index.row() == ARG2_PROPERTY)
			{
				if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
				{
					ArithmeticExprDropEvent(_it, _index, _suffix, _fileName, item(ARG1_PROPERTY, VALUE)->text());
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "TRANSLATABLE_OBJECT")
				{
					ArithmeticExprDropEvent(_it, _index, _suffix, _fileName, "VECTOR_3");
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "COLORABLE_OBJECT")
				{
					ArithmeticExprDropEvent(_it, _index, _suffix, _fileName, "VECTOR_3");
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "SCALABLE_OBJECT")
				{
					ArithmeticExprDropEvent(_it, _index, _suffix, _fileName, "VECTOR_3");
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "ROTATABLE_OBJECT")
				{
					ArithmeticExprDropEvent(_it, _index, _suffix, _fileName, "QUATERNION");
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
				{
					StringExprDropEvent(_it, _index, _suffix, _fileName);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
				{
					BooleanExprDropEvent(_it, _index, _suffix, _fileName);
				}
			}
		} 
		else if(item(_index.row(), ARG_MODE)->text() == "TEMPLATE")
		{
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}



void ActionCreator::CHANGE_LIST_VALUE_SetAction(int _actionType)
{
	if(_actionType == AbstractList::CHANGE_LIST_INTEGER_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListIntegerAction* derived = dynamic_cast<ChangeListIntegerAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "INTEGER");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, QString::number(derived->GetIntegerKeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_FLOAT_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListFloatAction* derived = dynamic_cast<ChangeListFloatAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "FLOAT");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, QString::number(derived->GetFloatKeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}  
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_BOOLEAN_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListBooleanAction* derived = dynamic_cast<ChangeListBooleanAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "BOOLEAN");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetBooleanKeyArg(AbstractAction::ARG1) ? "true" : "false");
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_STRING_MESSAGE)
	{ 
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListStringAction* derived = dynamic_cast<ChangeListStringAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "STRING");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetStringKeyArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_VECTOR_2_MESSAGE)
	{ 
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListVector2Action* derived = dynamic_cast<ChangeListVector2Action*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "VECTOR_2");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, Vector2ToString(derived->GetVector2KeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_VECTOR_3_MESSAGE)
	{ 
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListVector3Action* derived = dynamic_cast<ChangeListVector3Action*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "VECTOR_3");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, Vector3ToString(derived->GetVector3KeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_QUATERNION_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListQuaternionAction* derived = dynamic_cast<ChangeListQuaternionAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "QUATERNION");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, QuaternionToString(derived->GetQuaternionKeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_TRANSLATABLE_MESSAGE)
	{ 
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListTranslatableAction* derived = dynamic_cast<ChangeListTranslatableAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "TRANSLATABLE_OBJECT");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, Vector3ToString(derived->GetVector3KeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_SCALABLE_MESSAGE)
	{ 
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListScalableAction* derived = dynamic_cast<ChangeListScalableAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "SCALABLE_OBJECT");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, Vector3ToString(derived->GetVector3KeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_ROTATABLE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListRotatableAction* derived = dynamic_cast<ChangeListRotatableAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "ROTATABLE_OBJECT");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, QuaternionToString(derived->GetQuaternionKeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractList::CHANGE_LIST_COLORABLE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_LIST_VALUE");

		ChangeListColorableAction* derived = dynamic_cast<ChangeListColorableAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "COLORABLE_OBJECT");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, Vector3ToString(derived->GetVector3KeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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


void ActionCreator::CHANGE_LIST_VALUE_Create(void)
{ 
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "CHANGE_LIST_VALUE")
	{
		if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "INTEGER")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_INTEGER_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetIntegerKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toInt());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "FLOAT")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_FLOAT_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetFloatKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toFloat());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "BOOLEAN")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_BOOLEAN_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetBooleanKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE) == "true" ? true : false);
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "STRING")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_STRING_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetStringKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "VECTOR_2")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_VECTOR_2_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetVector2KeyArg(AbstractAction::ARG1, StringToVector2(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "VECTOR_3")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_VECTOR_3_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetVector3KeyArg(AbstractAction::ARG1, StringToVector3(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "QUATERNION")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_QUATERNION_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetQuaternionKeyArg(AbstractAction::ARG1, StringToQuaternion(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "TRANSLATABLE_OBJECT")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_TRANSLATABLE_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetVector3KeyArg(AbstractAction::ARG1, StringToVector3(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "SCALABLE_OBJECT")
		{ 
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_SCALABLE_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetVector3KeyArg(AbstractAction::ARG1, StringToVector3(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "ROTATABLE_OBJECT")
		{ 
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_ROTATABLE_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetQuaternionKeyArg(AbstractAction::ARG1, StringToQuaternion(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "COLORABLE_OBJECT")
		{
			action = Trigger::_CreateAction(AbstractList::CHANGE_LIST_COLORABLE_MESSAGE);

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
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetVector3KeyArg(AbstractAction::ARG1, StringToVector3(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
	} 
}
