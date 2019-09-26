#ifndef GETNORMALIZEDVECTOR3ACTION_H
#define GETNORMALIZEDVECTOR3ACTION_H
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
class GetNormalizedVector3Action: public AbstractAction
{
	friend class Trigger;
	friend class AbstractAction;
	private: StringANSI arg1Name;
    private: StringExprParserEx templateArg1;
    private: MagicPointer<Variable<Vector3>> arg1;
   
	private: StringANSI targetName;
	private: StringExprParserEx templateTarget;
	private: MagicPointer<Variable<Vector3>> target;
	private: GetNormalizedVector3Action(void);
	private: GetNormalizedVector3Action(int32 _type);
	public: virtual ~GetNormalizedVector3Action(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual void operator () (void);
	private: void TemplateArg1IsCompleted(void);
	private: void Arg1IsLost(void);
	private: void TemplateArg1IsChanged(void);
	private: void TemplateTargetIsCompleted(void);
	private: void TargetIsLost(void);
	private: void TemplateTargetIsChanged(void);
	private: void ObjectIsDestroyed(AbstractObject* _object);
	private: void TargetIsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg1IsRenamed(AbstractObject::NameInfo* _info);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void UpdateValid(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	private: static AbstractAction* _LoadFromFile(File& _file);
};


#endif 