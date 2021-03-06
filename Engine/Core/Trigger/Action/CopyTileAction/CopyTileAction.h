#ifndef COPYTILEACTION_H
#define COPYTILEACTION_H
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>
class CopyTileAction: public AbstractAction
{
	friend class Trigger;
	friend class AbstractAction;
	private: StringANSI arg1Name;
    private: StringExprParserEx templateArg1;
    private: MagicPointer<TranslatableObject> arg1;

	private: StringANSI arg2Name;
    private: StringExprParserEx templateArg2;
    private: MagicPointer<TranslatableObject> arg2;
  
    private: StringANSI arg3Name;
    private: StringExprParserEx templateArg3;
    private: MagicPointer<Variable<bool>> arg3;

	private: StringANSI targetName;
	private: StringExprParserEx templateTarget;
	private: MagicPointer<TiledSpace3D> target;
	private: CopyTileAction(void);
	private: CopyTileAction(int32 _type);
	public: virtual ~CopyTileAction(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetVector3KeyArg(int32 _index, Vector3 _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual Vector3 GetVector3KeyArg(int32 _index);
	public: virtual void operator () (void);
	private: void TemplateArg1IsCompleted(void);
	private: void Arg1IsLost(void);
	private: void TemplateArg1IsChanged(void);
	private: void TemplateArg2IsCompleted(void);
	private: void Arg2IsLost(void);
	private: void TemplateArg2IsChanged(void);
	private: void TemplateArg3IsCompleted(void);
	private: void Arg3IsLost(void);
	private: void TemplateArg3IsChanged(void);
	private: void TemplateTargetIsCompleted(void);
	private: void TargetIsLost(void);
	private: void TemplateTargetIsChanged(void);
	private: void ObjectIsDestroyed(AbstractObject* _object);
	private: void TargetIsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg1IsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg2IsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg3IsRenamed(AbstractObject::NameInfo* _info);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void UpdateValid(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	private: static AbstractAction* _LoadFromFile(File& _file);
};



#endif 