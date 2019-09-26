#ifndef CHECKEQUALITYACTION_H
#define CHECKEQUALITYACTION_H


// INCLUDES -----------------------------------------------------------------------------------------
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Core/Trigger/EqualityExprParserEx/EqualityExprParserEx.h>
// CLASS --------------------------------------------------------------------------------------------


// �����, ����������� �������� CHECK_EQUALITY_MESSAGE
class CheckEqualityAction: public AbstractAction// ������� �������� ��������
{
	// ������������� ������
	friend class Trigger;
	friend class AbstractAction;


	// DATA
	private: StringANSI arg1Name;// ��� ���������
    private: EqualityExprParserEx expr;// ���������, � ������� ������������ ��� ��������� 
    private: MagicPointer<Variable<bool>> arg1;// �������� ��������

	private: StringANSI arg2Name;// ��� ���������
    private: StringExprParserEx templateArg2;// ���������, � ������� ������������ ��� ��������� 
    private: MagicPointer<AbstractAction> arg2;// �������� ��������

	private: StringANSI arg3Name;// ��� ���������
    private: StringExprParserEx templateArg3;// ���������, � ������� ������������ ��� ��������� 
    private: MagicPointer<AbstractAction> arg3;// �������� ��������


	// METHODS
	// ����������� ��� ����������
	private: CheckEqualityAction(void);

		
	// ���������������� ����������� 
	private: CheckEqualityAction(int32 _type);


	// ����������
	public: virtual ~CheckEqualityAction(void);


	// ���������...
	// ������ ��������-���������� _name ��� ��������� _index
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);


	// ������ ��������� �������� _expr ��� ��������� _index
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);


	// ������ ��������� �������� _expr ��� ��������� _index
	public: virtual void SetExpressionArg(int32 _index, StringANSI _expr);


	// ���������� ��������� �������� _index
	public: virtual StringANSI GetTemplateArg(int32 _index);


	// ���������� ��������� �������� _index
	public: virtual StringANSI GetExpressionArg(int32 _index);


	// ���������� ����������-�������� _index
	public: virtual StringANSI GetVariableArg(int32 _index);


	// ���������...
	// ��������� ��������
	// ����������: ���� �������� �� ����������, �� ������� ����� ��������
	public: virtual void operator () (void);


	// ���������...
	// ������� �� ������ ����� �� ���������� ��������� ��� Arg1
	private: void Arg1IsLost(void);


	// ������� �� ��������� ���������� ��������� ��� Arg2
	private: void TemplateArg2IsCompleted(void);


	// ������� �� ������ ����� �� ���������� ��������� ��� Arg2
	private: void Arg2IsLost(void);


	// ������� �� ��������� ����� �� ���������� ��������� ��� Arg2
	private: void TemplateArg2IsChanged(void);


	// ������� �� ��������� ���������� ��������� ��� Arg3
	private: void TemplateArg3IsCompleted(void);


	// ������� �� ������ ����� �� ���������� ��������� ��� Arg3
	private: void Arg3IsLost(void);


	// ������� �� ��������� ����� �� ���������� ��������� ��� Arg3
	private: void TemplateArg3IsChanged(void);


	// ������� �� ����������� �������
	private: void ObjectIsDestroyed(AbstractObject* _object);


    // ������� �� �������������� ��������� ��������
	private: void Arg1IsRenamed(AbstractObject::NameInfo* _info);


    // ������� �� �������������� ��������� ��������
	private: void Arg2IsRenamed(AbstractObject::NameInfo* _info);


    // ������� �� �������������� ��������� ��������
	private: void Arg3IsRenamed(AbstractObject::NameInfo* _info);


	// ������� �� ��������� �������
	private: void ObjectIsCreated(AbstractObject* _object);


	// ��������� ���������� ��������
	private: void UpdateValid(void);


	// ����������...
	// ��������� �������� �� ���� _path, � ���������� true � ������ ������
	// ����������: ���� _path �������� ������ ������, �� �������� ����� ��������� �� �����, � ��������������� ���������� 
	public: virtual bool SaveToFile(StringANSI _path = "");


	// ��������� �������� � ����� ����� _file, � ���������� true � ������ ������
	public: virtual bool SaveToFile(File& _file);

				
	// ��������� �������� �� ����� _file, � ���������� �� ���� ��������� � ������ ������
	private: static AbstractAction* _LoadFromFile(File& _file);
};



#endif 