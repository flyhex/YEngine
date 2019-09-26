#include "EqualityExprParser.h"
#include <Engine/Core/Container/Stack/SimpleStack/SimpleStack.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Parser/Variable/Vector3Variable/Vector3Variable.h>
#include <Engine/Core/Parser/Variable/Vector2Variable/Vector2Variable.h>
#include <Engine/Core/Parser/Variable/QuaternionVariable/QuaternionVariable.h>


EqualityExprParser::EqualityExprParser(void)
{
	currentState = START_STATE;
	currentError = NO_ERRORS;
}


EqualityExprParser::~EqualityExprParser(void)
{
	Destroy();
}


int32 EqualityExprParser::_IsNameValid(StringANSI _name)
{
	if(!_name.empty())
	{
		for(char character, i = 0; i < _name.length(); ++i)
		{
			 character = _name[i];

			if(character >= 48 && character <= 57 ||
			   character >= 65 && character <= 90 ||
			   character >= 97 && character <= 122 ||
			   character == '_')
			{}
			else
			{ 
				return i; 
			}
		}
	}
	return -1;
}


int32 EqualityExprParser::PriorityOf(char _operation)
{
	switch(_operation)
	{
		case '|':
		{
			return 0;
		}
			
		case '^':
		{
			return 1;
		}

		case '&':
		{
			return 2;
		}

		case '=':
		case '#':
		{
			return 3;
		}
						
		case '>':
		case '<':
		case '[':
		case ']':
		{
			return 4;
		}

		case '+':
		case '-':
		{
			return 5;
		}

		case '*':
		case '/':
		case '%':
		{
			return 6;
		}

		case '!':
		case '@':
		case '~':
		{
			return 7;
		}
	}
	return -1;
}


StringANSI EqualityExprParser::GetEqualityExpr(void)const
{
	return equalityExpr;
}


EqualityExprParser::ErrorType EqualityExprParser::GetCurrentError(void)const
{
	return currentError;
}		
			

StringANSI EqualityExprParser::GetCurrentErrorString(void)const
{
	switch(currentError)
	{
		case NO_ERRORS:
		{
			return "NO_ERRORS";
		}

		case UNKNOWN_TOKEN:
		{
			return "UNKNOWN_TOKEN";
		}

		case EXPR_NOT_FOUND:
		{
			return "EXPR_NOT_FOUND";
		}

		case UNDEFINED_VARIABLE:
		{
			return "UNDEFINED_VARIABLE";
		}

		case UNKNOWN_EXPR:
		{
			return "UNKNOWN_EXPR";
		}

		case RIGHT_BRACKET_NOT_FOUND:
		{
			return "RIGHT_BRACKET_NOT_FOUND";
		}

		case LEFT_BRACKET_NOT_FOUND:
		{
			return "LEFT_BRACKET_NOT_FOUND";
		}

		case INVALID_OPERATION:
		{
			return "INVALID_OPERATION";
		}

		case INVALID_NUMBER:
		{
			return "INVALID_NUMBER";
		}

		case VERY_LONG_NUMBER:
		{
			return "VERY_LONG_NUMBER";
		}

		case INVALID_RESULT:
		{
			return "INVALID_RESULT";
		}
	}
	return "";
}



StringANSI EqualityExprParser::GetErrorArg(void)const
{
	return errorArg;
}


int32 EqualityExprParser::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}


	
EqualityExprParser::ErrorType EqualityExprParser::Parse(StringANSI _equalityExpr)
{ 

	currentState = START_STATE;
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
	equalityExpr = _equalityExpr;
	tokens.Clear();
	 
	if(_equalityExpr.length() > 0)
	{
		SimpleStack<Operation> stack;
		int32 lostBrackets = 0;
		char lastOperation = ' ';
		int32 lastOperationIndex = -1;
		char character;
		bool floatSeparator;
		for(int32 i = 0; i < _equalityExpr.length(); ++i)
		{
			character = _equalityExpr[i];

			if(character != ' ')
			{ 
				switch(character)
				{
					case '(':
					{ 
						if(currentState == VARIABLE_RECEIVED_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						} 
						lostBrackets++;
						stack.Push(Operation(StringANSI(1, character), i));
						break;
					}

					case ')':
					{ 
						if(currentState == OPERATION_RECEIVED_STATE)
						{	
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						else if(lostBrackets == 0)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = '(';
							return currentError = LEFT_BRACKET_NOT_FOUND;
						} 
						lostBrackets--;

						Operation operation = stack.Pop();

						while(operation.value[0] != '(')
						{
							tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							operation = stack.Pop();
						} 
						break;
					}

					case '~':
					{
						if(currentState == START_STATE)
						{ 
							currentState = OPERATION_RECEIVED_STATE;
							lastOperation = character;
							lastOperationIndex = i;

							stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{	
							if(lastOperation == '>' || lastOperation == '<' || lastOperation == '=')
							{
								tokens.Clear();
								currentState = ERROR_STATE;
								errorArg = character;
								errorArgIndex = i;
								return currentError = UNKNOWN_EXPR;
							} 
							if(stack.Peek().value[0] == character)
							{
								tokens.Clear();
								currentState = ERROR_STATE;
								errorArg = character;
								errorArgIndex = i;
								return currentError = UNKNOWN_EXPR;
							}
							lastOperation = character;
							lastOperationIndex = i;

							while(stack.IsNotEmpty() && PriorityOf(lastOperation) < PriorityOf(stack.Peek().value[0]))
							{ 
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							}
							stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						} 
						else
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						break;
					}

					case '!':
					{
						if(currentState == START_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}

						if(i + 1 < equalityExpr.length() && equalityExpr[i + 1] == '=')
						{
							currentState = OPERATION_RECEIVED_STATE;
							lastOperation = character;
							lastOperationIndex = i;
							while(stack.IsNotEmpty() && PriorityOf('#') <= PriorityOf(stack.Peek().value[0]))
							{
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							} 

							stack.Push(Operation(StringANSI(1, '#'), lastOperationIndex));
							i++;
						}
						else
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_TOKEN;
						}
						break;
					}
					case '>':
					{
						if(currentState == START_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						currentState = OPERATION_RECEIVED_STATE;
						lastOperation = character;
						lastOperationIndex = i;

						if(i + 1 < equalityExpr.length() && equalityExpr[i + 1] == '=')
						{
							while(stack.IsNotEmpty() && PriorityOf(']') <= PriorityOf(stack.Peek().value[0]))
							{
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							} 

							stack.Push(Operation(StringANSI(1, ']'), lastOperationIndex));
							i++;
						} 
						else
						{
							while(stack.IsNotEmpty() && PriorityOf(lastOperation) <= PriorityOf(stack.Peek().value[0]))
							{
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							} 
							stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						}
						break;
					}
					case '<':
					{
						if(currentState == START_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						currentState = OPERATION_RECEIVED_STATE;
						lastOperation = character;
						lastOperationIndex = i;

						if(i + 1 < equalityExpr.length() && equalityExpr[i + 1] == '=')
						{
							while(stack.IsNotEmpty() && PriorityOf('[') <= PriorityOf(stack.Peek().value[0]))
							{
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							} 

							stack.Push(Operation(StringANSI(1, '['), lastOperationIndex));
							i++;
						} 
						else
						{
							while(stack.IsNotEmpty() && PriorityOf(lastOperation) <= PriorityOf(stack.Peek().value[0]))
							{
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							} 
							stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						}
						break;
					} 

					case '|':
					case '&':
					case '^':

					case '=':
						
					case '*':
					case '/':
					case '%':
					{
						if(currentState == START_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						currentState = OPERATION_RECEIVED_STATE;
						lastOperation = character;
						lastOperationIndex = i;

						while(stack.IsNotEmpty() && PriorityOf(lastOperation) <= PriorityOf(stack.Peek().value[0]))
						{
							Operation operation = stack.Pop();
							tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
						} 
						stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						break;
					} 

					case '+':
					{ 
						if(currentState == START_STATE)
						{
							currentState = OPERATION_RECEIVED_STATE;
							lastOperation = character;
							lastOperationIndex = i;
							stack.Push(Operation(StringANSI(1, '!'), lastOperationIndex));
							break;
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{ 
							if(stack.Peek().value[0] == '!')
							{
								tokens.Clear();
								currentState = ERROR_STATE;
								errorArg = character;
								errorArgIndex = i;
								return currentError = UNKNOWN_EXPR;
							}

							currentState = OPERATION_RECEIVED_STATE;
							lastOperation = character;
							lastOperationIndex = i;
							while(stack.IsNotEmpty() && PriorityOf('!') < PriorityOf(stack.Peek().value[0]))
							{
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							} 
							stack.Push(Operation(StringANSI(1, '!'), lastOperationIndex));
							break;
						}
						currentState = OPERATION_RECEIVED_STATE;

						lastOperation = character;
						lastOperationIndex = i;

						while(stack.IsNotEmpty() && PriorityOf(lastOperation) <= PriorityOf(stack.Peek().value[0]))
						{ 
							Operation operation = stack.Pop();
							tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
						} 
						stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						break;
					} 

					case '-':
					{ 
						if(currentState == START_STATE)
						{
							currentState = OPERATION_RECEIVED_STATE;
							lastOperation = character;
							lastOperationIndex = i;
							stack.Push(Operation(StringANSI(1, '@'), lastOperationIndex));
							break;
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{ 
							if(stack.Peek().value[0] == '@')
							{
								tokens.Clear();
								currentState = ERROR_STATE;
								errorArg = character;
								errorArgIndex = i;
								return currentError = UNKNOWN_EXPR;
							}

							currentState = OPERATION_RECEIVED_STATE;
							lastOperation = character;
							lastOperationIndex = i;
							while(stack.IsNotEmpty() && PriorityOf('@') < PriorityOf(stack.Peek().value[0]))
							{
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							} 
							stack.Push(Operation(StringANSI(1, '@'), lastOperationIndex));
							break;
						}
						currentState = OPERATION_RECEIVED_STATE;
						
						lastOperation = character;
						lastOperationIndex = i;

						while(stack.IsNotEmpty() && PriorityOf(lastOperation) <= PriorityOf(stack.Peek().value[0]))
						{ 
							Operation operation = stack.Pop();
							tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
						} 
						stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						break;
					}

					default:
					{
						if(character >= 65 && character <= 90 ||
						   character >= 97 && character <= 122 ||
						   character == '_')
						{
							if(currentState == VARIABLE_RECEIVED_STATE)
							{
								if(_equalityExpr[i - 1] == ' ' ||
								   _equalityExpr[i - 1] == ')')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else if(currentState == NUMBER_RECEIVED_STATE)
							{ 
								if(_equalityExpr[i - 1] == ' ' ||
								   _equalityExpr[i - 1] == ')')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character; 
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								if(tokens.GetTail()->key.type == Token::FLOAT_NUMBER)
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								currentState = VARIABLE_RECEIVED_STATE;
								tokens.GetTail()->key.type = Token::VARIABLE;
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else
							{
								currentState = VARIABLE_RECEIVED_STATE;
								tokens.AddToTail(Token(StringANSI(1, character), Token::VARIABLE, i));
							}
						}
						else if(character >= 48 && character <= 57 ||
								character == '.')
						{
							if(currentState == VARIABLE_RECEIVED_STATE)
							{
								if(character == '.' ||
								   _equalityExpr[i - 1] == ' ' ||
								   _equalityExpr[i - 1] == ')')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else if(currentState == NUMBER_RECEIVED_STATE)
							{
								if(_equalityExpr[i - 1] == ' ' ||
								   _equalityExpr[i - 1] == ')')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								if(character == '.')
								{
									if(floatSeparator)
									{
										tokens.Clear();
										currentState = ERROR_STATE;
										errorArg = character;
										errorArgIndex = i;
										return currentError = INVALID_NUMBER;
									}
									tokens.GetTail()->key.type = Token::FLOAT_NUMBER;
									floatSeparator = true;
								}
								if(tokens.GetTail()->key.value.length() > 7)
								{
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									tokens.Clear();
									return currentError = VERY_LONG_NUMBER;
								}
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else
							{
								if(character == '.')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = INVALID_NUMBER;
								}
								currentState = NUMBER_RECEIVED_STATE;
								tokens.AddToTail(Token(StringANSI(1, character), Token::INT_NUMBER, i));
								floatSeparator = false;
							}
						}
						else
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_TOKEN;
						}
						break;
					}
				}
			}
		}
		while(stack.IsNotEmpty())
		{ 
			Operation operation = stack.Pop();
			tokens.AddToTail(Token(operation.value, Token::OPERATION, operation.position));
		}
				
		if(tokens.IsEmpty())
		{
			currentState = ERROR_STATE;
			return currentError = EXPR_NOT_FOUND;
		}

		if(currentState == OPERATION_RECEIVED_STATE)
		{
			tokens.Clear();
			currentState = ERROR_STATE;
			errorArg = lastOperation;
			errorArgIndex = lastOperationIndex;
			return currentError = UNKNOWN_EXPR;
		}
				 
		if(lostBrackets)
		{
			tokens.Clear();
			currentState = ERROR_STATE;
			errorArg = ')';
			return currentError = RIGHT_BRACKET_NOT_FOUND;
		}
	} 
	else
	{
		currentState = ERROR_STATE;
		return currentError = EXPR_NOT_FOUND;
	}
	currentState = END_STATE;
	return currentError;
}


bool EqualityExprParser::GetResult(void)const
{
	return result;
}
	

SimpleList<EqualityExprParser::Token> EqualityExprParser::GetTokens(void)const
{
	return tokens;
}



EqualityExprParser::ErrorType EqualityExprParser::Calculate(void)
{ 
	if(tokens.IsNotEmpty())
	{
		SimpleStack<AbstractVariable*> result;
		MagicList<AbstractVariable> deletionList;
		AbstractVariable* leftOperand;
		AbstractVariable* rightOperand;
		AbstractVariable* it;
		for(SimpleList<Token>::Node* token = tokens.GetHead(); token != NIL; token = token->GetRight())
		{
			if(token->key.type == Token::VARIABLE)
			{
				it = dynamic_cast<AbstractVariable*>(AssetLibrary::_GetAssets().FindObject(token->key.value));
					 
				if(it)
				{
					result.Push(it);
				}
				else
				{
					errorArg = token->key.value;
					return currentError = UNDEFINED_VARIABLE;
				}
			}
			else if(token->key.type == Token::FLOAT_NUMBER)
			{ 
				it = new Variable<float>(StringToFloat(token->key.value));
				deletionList.AddToTail(it);
				result.Push(it);
			}
			else if(token->key.type == Token::INT_NUMBER)
			{ 
				it = new Variable<int32>(StringToInt(token->key.value));
				deletionList.AddToTail(it);
				result.Push(it);
			}
			else if(token->key.type == Token::OPERATION)
			{
				char operation = token->key.value[0];
					
				if(operation == '~' ||
				   operation == '@' || 
				   operation == '!')
				{
					rightOperand = result.Pop();
						
					if(it = Calculate((OperationType)operation, rightOperand))
					{
						deletionList.AddToTail(it);
						result.Push(it);
					}
					else
					{ 
						errorArg = token->key.value;
						return currentError = INVALID_OPERATION;
					}
				}
				else
				{
					rightOperand = result.Pop();
					leftOperand = result.Pop();

					if(it = Calculate((OperationType)operation, leftOperand, rightOperand))
					{
						deletionList.AddToTail(it);
						result.Push(it);
					}
					else
					{ 
						errorArg = token->key.value;
						return currentError = INVALID_OPERATION;
					}
				}
			}
		} 
		it = result.Pop();

		if(it->GetValueType() != Variable<bool>::BOOLEAN_TYPE)
		{
			return currentError = INVALID_RESULT;
		}
		else
		{
			EqualityExprParser::result = dynamic_cast<Variable<bool>*>(it)->GetValue();
			currentError = NO_ERRORS;
		}
	}
	return currentError;
} 

	

AbstractVariable* EqualityExprParser::Calculate(OperationType _operation, AbstractVariable* _right)
{
	switch(_operation)
	{
		case NOT:
		{		
			if(_right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(!dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}

		case INVERSION:
		{
			if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				return new Variable<float>(-dynamic_cast<Variable<float>*>(_right)->GetValue());
			}
			else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				return new Variable<int32>(-dynamic_cast<Variable<int32>*>(_right)->GetValue());
			}
			else if(_right->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
			{
				return new Variable<Vector2>(Vector2::_GetInverse(dynamic_cast<Variable<Vector2>*>(_right)->GetValue()));
			}
			else if(_right->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
			{
				return new Variable<Vector3>(Vector3::_GetInverse(dynamic_cast<Variable<Vector3>*>(_right)->GetValue()));
			}
			else if(_right->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
			{
				return new Variable<Quaternion>(Quaternion::_GetConjugate(dynamic_cast<Variable<Quaternion>*>(_right)->GetValue()));
			}
			break;
		}
		
		case ABS:
		{
			if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				return new Variable<float>(fabs(dynamic_cast<Variable<float>*>(_right)->GetValue()));
			}
			else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				return new Variable<int32>(abs(dynamic_cast<Variable<int32>*>(_right)->GetValue()));
			}
			break;
		}
	}
	return NIL;
}


AbstractVariable* EqualityExprParser::Calculate(OperationType _operation, AbstractVariable* _left, AbstractVariable* _right)
{
	switch(_operation)
	{	
		case EQUAL:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() == dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() == (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() == dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() == (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() == dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE && _right->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() == dynamic_cast<Variable<Vector2>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE && _right->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() == dynamic_cast<Variable<Vector3>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE && _right->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<Quaternion>*>(_left)->GetValue() == dynamic_cast<Variable<Quaternion>*>(_right)->GetValue());
			}
			break;
		}

		case NOT_EQUAL:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() != dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() != (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() != dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() != (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() != dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE && _right->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() != dynamic_cast<Variable<Vector2>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE && _right->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() != dynamic_cast<Variable<Vector3>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE && _right->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<Quaternion>*>(_left)->GetValue() != dynamic_cast<Variable<Quaternion>*>(_right)->GetValue());
			}
			break;
		}

		case GREATER:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() > dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() > (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() > dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() > (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() > dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}

		case GREATER_OR_EQUAL:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() >= dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() >= (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() >= dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() >= (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() >= dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}
		
		case LESS:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() < dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() < (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() < dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() < (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() < dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}

		case LESS_OR_EQUAL:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() <= dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<float>*>(_left)->GetValue() <= (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() <= dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<bool>(dynamic_cast<Variable<int32>*>(_left)->GetValue() <= (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() <= dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}
		
		case OR:
		{
			if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() | dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}
							
		case AND:
		{
			if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() & dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}
			
		case XOR:
		{
			if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() ^ dynamic_cast<Variable<bool>*>(_right)->GetValue());			
			}
			break;
		}

		case MULTIPLY:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<float>(dynamic_cast<Variable<float>*>(_left)->GetValue() * dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<float>(dynamic_cast<Variable<float>*>(_left)->GetValue() * (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() * dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() * (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
			{
				if(_right->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
				{
					if(_left == _right)
					{
						return new Variable<Vector2>(Vector2::_GetCross(dynamic_cast<Variable<Vector2>*>(_left)->GetValue()));
					}
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<Vector2>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() * dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<Vector2>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() * (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
			{
				if(_right->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
				{
					return new Variable<Vector3>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() * dynamic_cast<Variable<Vector3>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
				{
					return new Variable<Vector3>(dynamic_cast<Variable<Quaternion>*>(_right)->GetValue() * dynamic_cast<Variable<Vector3>*>(_left)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<Vector3>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() * dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<Vector3>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() * (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
			{
				if(_right->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
				{
					return new Variable<Quaternion>(dynamic_cast<Variable<Quaternion>*>(_left)->GetValue() * dynamic_cast<Variable<Quaternion>*>(_right)->GetValue());
				}
			}
			break;
		}
			
		case REMAINDER:
		{
			if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() % dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			break;
		}

		case DIVIDE:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					float value = dynamic_cast<Variable<float>*>(_right)->GetValue();
					if(value == 0.0f) { return NIL; }
					return new Variable<float>(dynamic_cast<Variable<float>*>(_left)->GetValue() / value);
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					float value = (float)dynamic_cast<Variable<int32>*>(_right)->GetValue();
					if(value == 0.0f) { return NIL; }
					return new Variable<float>(dynamic_cast<Variable<float>*>(_left)->GetValue() / value);
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					int32 value = dynamic_cast<Variable<int32>*>(_right)->GetValue();
					if(value == 0) { return NIL; }
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() / value);
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					int32 value = (int32)dynamic_cast<Variable<float>*>(_right)->GetValue();
					if(value == 0) { return NIL; }
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() / value);
				}
			}
			else if(_left->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
			{
				if(_right->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
				{
					return new Variable<float>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() / dynamic_cast<Variable<Vector2>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<Vector2>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() / dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<Vector2>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() / (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
			{
				if(_right->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
				{
					return new Variable<float>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() / dynamic_cast<Variable<Vector3>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<Vector3>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() / dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<Vector3>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() / (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			} 
			break;
		}

		case PLUS:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<float>(dynamic_cast<Variable<float>*>(_left)->GetValue() + dynamic_cast<Variable<float>*>(_right)->GetValue());			
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<float>(dynamic_cast<Variable<float>*>(_left)->GetValue() + (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());			
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() + dynamic_cast<Variable<int32>*>(_right)->GetValue());			
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() + (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());			
				}
			}
			else if(_left->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE && _right->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
			{
				return new Variable<Vector2>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() + dynamic_cast<Variable<Vector2>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE && _right->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
			{
				return new Variable<Vector3>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() + dynamic_cast<Variable<Vector3>*>(_right)->GetValue());
			}
			break;
		}
		
		case MINUS:
		{
			if(_left->GetValueType() == Variable<float>::FLOAT_TYPE)
			{
				if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<float>(dynamic_cast<Variable<float>*>(_left)->GetValue() - dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<float>(dynamic_cast<Variable<float>*>(_left)->GetValue() - (float)dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() - dynamic_cast<Variable<int32>*>(_right)->GetValue());
				}
				else if(_right->GetValueType() == Variable<float>::FLOAT_TYPE)
				{
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() - (int32)dynamic_cast<Variable<float>*>(_right)->GetValue());
				}
			}
			else if(_left->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE && _right->GetValueType() == Variable<Vector2>::VECTOR_2_TYPE)
			{
				return new Variable<Vector2>(dynamic_cast<Variable<Vector2>*>(_left)->GetValue() - dynamic_cast<Variable<Vector2>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE && _right->GetValueType() == Variable<Vector3>::VECTOR_3_TYPE)
			{
				return new Variable<Vector3>(dynamic_cast<Variable<Vector3>*>(_left)->GetValue() - dynamic_cast<Variable<Vector3>*>(_right)->GetValue());
			}
			else if(_left->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE && _right->GetValueType() == Variable<Quaternion>::QUATERNION_TYPE)
			{
				return new Variable<Quaternion>(Quaternion(dynamic_cast<Variable<Quaternion>*>(_right)->GetValue(), dynamic_cast<Variable<Quaternion>*>(_left)->GetValue()));
			} 
			break;
		}
	}
	return NIL;
}


void EqualityExprParser::Destroy(void)
{
	currentState = START_STATE;
	tokens.Clear();
	equalityExpr.clear();
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
}




bool EqualityExprParser::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(equalityExpr.length());
		_file.WriteString(equalityExpr);
		_file.Write(currentState);
		_file.Write(currentError);
		_file.Write(errorArg.length());
		_file.WriteString(errorArg);
		_file.Write(errorArgIndex);
		_file.Write(tokens.GetSize());
		for(SimpleList<Token>::Node* it = tokens.GetHead(); it != NIL; it = it->GetRight())
		{
			_file.Write(it->key.value.length());
			_file.WriteString(it->key.value);
			_file.Write(it->key.type);
			_file.Write(it->key.position);
		}
		return true;
	}
	return false;
}


bool EqualityExprParser::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		Destroy();
		int32 length;
		_file.Read(length);
		_file.ReadString(equalityExpr, length);
		_file.Read(currentState);
		_file.Read(currentError);
		_file.Read(length);
		_file.ReadString(errorArg, length);
		_file.Read(errorArgIndex);
		StringANSI value;
		Token::TokenType type;
		int32 position;
		int32 count;
		_file.Read(count);
		for(int32 i = 0; i < count; ++i)
		{
			_file.Read(length);
			_file.ReadString(value, length);
			_file.Read(type);
			_file.Read(position);

			tokens.AddToTail(Token(value, type, position));
		}
		return true;
	}
	return false;
}