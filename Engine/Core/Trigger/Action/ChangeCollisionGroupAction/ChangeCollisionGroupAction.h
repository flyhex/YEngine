#ifndef CHANGECOLLISIONGROUPACTION_H
#define CHANGECOLLISIONGROUPACTION_H
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
class ChangeCollisionGroupAction: public AbstractAction
{
	friend class Trigger;
	friend class AbstractAction;
	private: StringANSI argName;
    private: StringExprParserEx templateArg;
    private: MagicPointer<Variable<int32>> arg;

	private: StringANSI targetName;
	private: StringExprParserEx templateTarget;
	private: MagicPointer<PolygonalSurface> target;
	private: ChangeCollisionGroupAction(void);
	private: ChangeCollisionGroupAction(int32 _type);
	public: virtual ~ChangeCollisionGroupAction(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetIntegerKeyArg(int32 _index, int32 _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual int32 GetIntegerKeyArg(int32 _index);
	public: virtual void operator () (void);
	private: void TemplateArgIsCompleted(void);
	private: void ArgIsLost(void);
	private: void TemplateArgIsChanged(void);
	private: void TemplateTargetIsCompleted(void);
	private: void TargetIsLost(void);
	private: void TemplateTargetIsChanged(void);
	private: void ObjectIsDestroyed(AbstractObject* _object);
	private: void TargetIsRenamed(AbstractObject::NameInfo* _info);
	private: void ArgIsRenamed(AbstractObject::NameInfo* _info);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void UpdateValid(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	private: static AbstractAction* _LoadFromFile(File& _file);
};


#endif 