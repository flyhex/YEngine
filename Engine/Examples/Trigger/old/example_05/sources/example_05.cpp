// Win32.cpp: ���������� ����� ����� ��� ����������.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Material/AnimatedMaterial/AnimatedMaterial.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Helpers/Sprite/ColoredSprite/ColoredSprite.h>



// ���������������� ����������
class Application: public Engine,// ������
         	       public MessageHandler<int32>::Receiver// ���������� ���������
{
	// DATA
	private: Camera3D* camera;// ������
    private: Font2D *font2d;// �����
	private: Scene* scene;// �����


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL)
	{}

			
	// ����������
	public: virtual ~Application(void)
	{} 



	// ������� ����������
	public: bool Create(Engine::Define _define)
	{
		// ������� ������...
		if(Engine::Create(_define))
		{
			// �����...
			{
				scene = new Scene();
				scene->Rename("scene");// ��� �����
				AssetLibrary::_GetAssets().AddObject(scene);// �������� ����� � ���������� �������, ����� �� ����������
			}


			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 20;// ������ ������
				def.fontColor = RGBA(RGBA::WHITE_COLOR);// ���� ������

				font2d = new Font2D();
				font2d->Rename("font2d");
				font2d->Create(def);
				scene->AddObject(font2d);// �������� � �����
				font2d->Pick();// ������� �����
			}



			// ������...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// ����� �������
				def.zoomEnabled = false;
				def.rotationEnabled = false;

				camera = new Camera3D();
				camera->Create(def);
				
				camera->Pick(Title()); // ������� ������ 
				
				AssetLibrary::_GetAssets().AddObject(camera);// �������� � �����
			}
 
			// ������� ������� �����...
			{
				InputHandler* handler = new InputHandler();
			
				Mouse::Event* evt1 = new Mouse::Event();
				evt1->key = Mouse::LEFT_BUTTON;
				evt1->type = Mouse::KEY_DOWN;
				handler->AddEvent(evt1);
				
				handler->Rename("leftClick");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}

			// ����������...
			{ 
				// ��� ������
				Variable<StringANSI>* var1 = new Variable<StringANSI>("run_right_source");
				var1->Rename("atlasVar"); 
				AssetLibrary::_GetAssets().AddObject(var1);	


				// ��� ������
				Variable<int32>* var2 = new Variable<int32>(AssetLibrary::ATLAS_2D_ASSET);
				var2->Rename("atlasTypeVar"); 
				AssetLibrary::_GetAssets().AddObject(var2);	


				// ��������� ���������
				Variable<bool>* var3 = new Variable<bool>(true);
				var3->Rename("enableVar"); 
				AssetLibrary::_GetAssets().AddObject(var3);	

				
				// ��� ��������
				Variable<StringANSI>* var4 = new Variable<StringANSI>("run_right_anim");
				var4->Rename("animVar"); 
				AssetLibrary::_GetAssets().AddObject(var4);	


				// ��� ��������
				Variable<int32>* var5 = new Variable<int32>(AssetLibrary::TEXTURED_ANIMATION_ASSET);
				var5->Rename("animTypeVar"); 
				AssetLibrary::_GetAssets().AddObject(var5);	

					
				// ������ ����� ��������
				Variable<Vector2D>* var6 = new Variable<Vector2D>(Vector2D());
				var6->Rename("frameSize");
				AssetLibrary::_GetAssets().AddObject(var6);
				
			
				// ��� �������� play
				Variable<StringANSI>* var7 = new Variable<StringANSI>("play");
				var7->Rename("playVar"); 
				AssetLibrary::_GetAssets().AddObject(var7);	


				// ��� ��������
				Variable<int32>* var8 = new Variable<int32>(AssetLibrary::TEXTURE_2D_ASSET);
				var8->Rename("textureTypeVar"); 
				AssetLibrary::_GetAssets().AddObject(var8);	

								
				// ������� ������ Play
				Variable<Vector3D>* var9 = new Variable<Vector3D>(Vector3D(-0.15f, -0.25f, 0));
				var9->Rename("playButtonPosition");
				AssetLibrary::_GetAssets().AddObject(var9);

								
				// ������ ����������� ������� � ������ Play...
				Variable<bool>* var10 = new Variable<bool>(false);
				var10->Rename("playButtonUnderCursor");
				AssetLibrary::_GetAssets().AddObject(var10);

						
	
				// ��������� �������� play
				Variable<int32>* var12 = new Variable<int32>(AbstractAnimation::PLAY);
				var12->Rename("playModeVar"); 
				AssetLibrary::_GetAssets().AddObject(var12);	

				
				// ���� �������������� ������
				Variable<RGBA>* var13 = new Variable<RGBA>(RGBA(RGBA::GREEN_COLOR));
				var13->Rename("activatedColorVar");
				AssetLibrary::_GetAssets().AddObject(var13);
				

				// ���� �������������� ������
				Variable<RGBA>* var14 = new Variable<RGBA>(RGBA(RGBA::WHITE_COLOR));// ������� ����
				var14->Rename("deactivatedColorVar");
				AssetLibrary::_GetAssets().AddObject(var14);

								
				// ��� �������� pause
				Variable<StringANSI>* var15 = new Variable<StringANSI>("pause");
				var15->Rename("pauseVar"); 
				AssetLibrary::_GetAssets().AddObject(var15);	

								
				// ������� ������ Pause
				Variable<Vector3D>* var16 = new Variable<Vector3D>(Vector3D(-0.02, -0.25f, 0));
				var16->Rename("pauseButtonPosition");
				AssetLibrary::_GetAssets().AddObject(var16);


				// ��������� �������� pause
				Variable<int32>* var17 = new Variable<int32>(AbstractAnimation::PAUSE);
				var17->Rename("pauseModeVar"); 
				AssetLibrary::_GetAssets().AddObject(var17);	


				// ������ ����������� ������� � ������ Pause...
				Variable<bool>* var18 = new Variable<bool>(false);
				var18->Rename("pauseButtonUnderCursor");
				AssetLibrary::_GetAssets().AddObject(var18);

				
				// ��� �������� stop
				Variable<StringANSI>* var19 = new Variable<StringANSI>("stop");
				var19->Rename("stopVar"); 
				AssetLibrary::_GetAssets().AddObject(var19);	

				
				// ������� ������ Stop
				Variable<Vector3D>* var20 = new Variable<Vector3D>(Vector3D(0.11, -0.25f, 0));
				var20->Rename("stopButtonPosition");
				AssetLibrary::_GetAssets().AddObject(var20); 
					

				// ������ ����������� ������� � ������ Stop...
				Variable<bool>* var21 = new Variable<bool>(false);
				var21->Rename("stopButtonUnderCursor");
				AssetLibrary::_GetAssets().AddObject(var21);

	
				// ��������� �������� stop
				Variable<int32>* var22 = new Variable<int32>(AbstractAnimation::STOP);
				var22->Rename("stopModeVar"); 
				AssetLibrary::_GetAssets().AddObject(var22);	

								
				// ������ �������� �����
				Variable<int32>* var23 = new Variable<int32>(0);
				var23->Rename("currentFrameIndex"); 
				AssetLibrary::_GetAssets().AddObject(var23);


				// ��� �������
				Variable<int32>* var24 = new Variable<int32>(AssetLibrary::OBJECT_ASSET);
				var24->Rename("objectTypeVar"); 
				AssetLibrary::_GetAssets().AddObject(var24);


				// ��� 
				Variable<StringANSI>* var25 = new Variable<StringANSI>;
				var25->Rename("newName"); 
				AssetLibrary::_GetAssets().AddObject(var25);	

				
				// ������� ��� Frame...
				Variable<Vector3D>* var26 = new Variable<Vector3D>(Vector3D(-0.70f, -0.35f, 0));
				var26->Rename("framePosition"); 
				AssetLibrary::_GetAssets().AddObject(var26);

				
				// �������� ��� Frame...
				Variable<Vector3D>* var27 = new Variable<Vector3D>((Vector3D(100, 0, 0) / 100.0 / 27.0) + Vector3D(0.011,0,0));
				var27->Rename("offset"); 
				AssetLibrary::_GetAssets().AddObject(var27);

					
				// ��� �����
				Variable<StringANSI>* var28 = new Variable<StringANSI>("quad");
				var28->Rename("quadVar");
				AssetLibrary::_GetAssets().AddObject(var28);


				// ��� ��������
				Variable<int32>* var29 = new Variable<int32>(AssetLibrary::TRIGGER_ASSET);
				var29->Rename("triggerTypeVar"); 
				AssetLibrary::_GetAssets().AddObject(var29);


				// ��� ��������
				Variable<StringANSI>* var30 = new Variable<StringANSI>("checkFrameUnderCursor");
				var30->Rename("checkFrameUnderCursorVar");
				AssetLibrary::_GetAssets().AddObject(var30);

					
				// ��� ��������
				Variable<StringANSI>* var31 = new Variable<StringANSI>;
				var31->Rename("triggerName");
				AssetLibrary::_GetAssets().AddObject(var31);

				
				// ��� ������� ���������
				Variable<StringANSI>* var32 = new Variable<StringANSI>;
				var32->Rename("triggerEventExpr");
				AssetLibrary::_GetAssets().AddObject(var32);

					
				// ��� �������
				Variable<StringANSI>* var33 = new Variable<StringANSI>;
				var33->Rename("triggerActionExpr");
				AssetLibrary::_GetAssets().AddObject(var33);


				// ��� ��������
				Variable<StringANSI>* var34 = new Variable<StringANSI>("currentFrame");
				var34->Rename("currentFrameVar");
				AssetLibrary::_GetAssets().AddObject(var34);


				// ��� �������
				Variable<StringANSI>* var35 = new Variable<StringANSI>("pickFrame");
				var35->Rename("pickFrameVar");
				AssetLibrary::_GetAssets().AddObject(var35);


				// ��� ��������
				Variable<int32>* var36 = new Variable<int32>(AssetLibrary::ACTION_ASSET);
				var36->Rename("actionTypeVar");
				AssetLibrary::_GetAssets().AddObject(var36);


				// ��� �������
				Variable<int32>* var37 = new Variable<int32>(AssetLibrary::EVENT_ASSET);
				var37->Rename("eventTypeVar");
				AssetLibrary::_GetAssets().AddObject(var37);

				// ����
				Variable<int32>* var38 = new Variable<int32>(0);
				var38->Rename("zero");
				AssetLibrary::_GetAssets().AddObject(var38);

				
				// ���������� �������� NOT
				Variable<StringANSI>* var39 = new Variable<StringANSI>("~");
				var39->Rename("NOT");
				AssetLibrary::_GetAssets().AddObject(var39);

				
				// ��� �������
				Variable<StringANSI>* var40 = new Variable<StringANSI>("unpickFrame");
				var40->Rename("unpickFrameVar");
				AssetLibrary::_GetAssets().AddObject(var40);


			    // ��� ��������
				Variable<StringANSI>* var41 = new Variable<StringANSI>("setFrame");
				var41->Rename("setFrameVar");
				AssetLibrary::_GetAssets().AddObject(var41);
			}
		


			{
				// ������� �������� ��������...
				Object* runner = Object::_CreateObject(Object::_CreateObjectType(IAnimatedSprite::ANIMATED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				runner->Rename("runner");
				AssetLibrary::_GetAssets().AddObject(runner);

				AnimatedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<AnimatedSprite<Vector3D, Quaternion, Matrix4x4>*>(runner);
				scene->AddGraphicsObject(sprite); // �������� � ����������� �����
				//scene->AddLogicObject(sprite); // �������� � ���������� �����
			}


			{ 
				// �������� �������� ��������...
				Trigger* animLoader1 = new Trigger();
			    animLoader1->LoadFromFile("animLoader1");
				
			
				/*
				
				
				animLoader1->Rename("animLoader1");
				animLoader1->SetActionExpr("loadAtlas, enableAnimatedMaterial, loadAnimation, changeAtlas, getFrameSize, changeAnimation, addFrameToLogicScene");
						

				// ��������
				IAction* action1 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "run_right_source", AssetLibrary::ATLAS_2D_ASSET);
				action1->Rename("loadAtlas");
				AssetLibrary::_GetAssets().AddObject(action1);	
			
	 
				// ��������
				IAction* action2 = Trigger::_CreateAction(IAnimatedMaterial::ENABLE_MATERIAL_MESSAGE, "runner", true);// �������� ���������������� ��������
				action2->Rename("enableAnimatedMaterial");
				AssetLibrary::_GetAssets().AddObject(action2);
				

			
				// ��������
				IAction* action3 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "run_right_source_anim", AssetLibrary::TEXTURED_ANIMATION_ASSET);
				action3->Rename("loadAnimation");
				AssetLibrary::_GetAssets().AddObject(action3);			
			
							
			    
				// ��������
				IAction* action4 = Trigger::_CreateAction(TexturedAnimation::CHANGE_ATLAS_MESSAGE, "run_right_source_anim", "run_right_source");
				action4->Rename("changeAtlas");
				AssetLibrary::_GetAssets().AddObject(action4);	
			
					
				
				// �������� 
				IAction* action5 = Trigger::_CreateAction(TexturedAnimation::GET_FRAME_SIZE_MESSAGE, "run_right_source_anim", "frameSize");
				action5->Rename("getFrameSize");
				AssetLibrary::_GetAssets().AddObject(action5);	  

				

				// ��������
				IAction* action6 = Trigger::_CreateAction(IAnimatedMaterial::CHANGE_ANIMATION_MESSAGE, "runner", "run_right_source_anim");
				action6->Rename("changeAnimation");
				AssetLibrary::_GetAssets().AddObject(action6);	


				// ��������
				IAction* action7 = Trigger::_CreateAction(LogicScene::MOVE_TO_LOGIC_SCENE_MESSAGE, "scene", "run_right_source_anim");
				action7->Rename("addFrameToLogicScene");  
				AssetLibrary::_GetAssets().AddObject(action7);	
		
				
				animLoader1->SetEventExpr(""); 
				animLoader1->SaveToFile();
				*/ 
				
				AssetLibrary::_GetAssets().AddObject(animLoader1); 


	

				Trigger* animLoader2 = new Trigger();
				animLoader2->LoadFromFile("animLoader2");

	
				/*
			
				animLoader2->Rename("animLoader2");
				animLoader2->SetActionExpr("changeValue1, createQuad, changeValue2");

				
				// �������
				IEvent* event1 = Trigger::_CreateEvent(IAnimatedMaterial::ENABLE_MATERIAL_MESSAGE, "runner", true);
				event1->Rename("isMaterialEnable"); 
				AssetLibrary::_GetAssets().AddObject(event1); 
					
				
			 
	
				// �������
				IEvent* event2 = Trigger::_CreateEvent(IAnimatedMaterial::CHANGE_ANIMATION_MESSAGE, "runner", "run_right_source_anim");
				event2->Rename("isChangeAnimation");
				AssetLibrary::_GetAssets().AddObject(event2);
				 
		
				// �������
				IEvent* event3 = Trigger::_CreateEvent(TexturedAnimation::CHANGE_ATLAS_MESSAGE, "run_right_source_anim", "run_right_source");
				event3->Rename("isChangeAtlas"); 
				AssetLibrary::_GetAssets().AddObject(event3);
			
			
 
				// ��������
				IAction* action8 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "frameSize", "frameSize / 100.0 / 3.0", IVariable::_GetValueType<Vector2D>());
				action8->Rename("changeValue1");
				AssetLibrary::_GetAssets().AddObject(action8);	


				
				// ��������
				IAction* action9 = Trigger::_CreateTemplateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "#runner", "#frameSize");
				action9->Rename("createQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	
 

								
				// ��������
				IAction* action10 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "frameSize", "frameSize / 4.0", IVariable::_GetValueType<Vector2D>());
				action10->Rename("changeValue2"); 
				AssetLibrary::_GetAssets().AddObject(action10);	
			
				
				 
				animLoader2->SetEventExpr("isMaterialEnable & isChangeAnimation & isChangeAtlas");
				animLoader2->SaveToFile();
				*/

				AssetLibrary::_GetAssets().AddObject(animLoader2); 
				
			}



			

			{
				// ������� ������ Play...
				Object* playButton = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				playButton->Rename("playButton");
				AssetLibrary::_GetAssets().AddObject(playButton);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(playButton);
				scene->AddGraphicsObject(sprite); // �������� � ����������� �����
			}


			{
				// �������� ������ Play...
				Trigger* playButtonLoader = new Trigger();
				playButtonLoader->LoadFromFile("playButtonLoader");
				
				
				/*
				
				playButtonLoader->Rename("playButtonLoader");
				playButtonLoader->SetActionExpr("playButtonLoadTexture, playButtonEnableTexturedMaterial, playButtonChangeTexture, playButtonCreateQuad, playButtonReplacePosition");

				
				// ������� 
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "animLoader2");// ���� ������
				event1->Rename("runnerInit2");
				AssetLibrary::_GetAssets().AddObject(event1);



				
				// ��������
				IAction* action1 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "play", AssetLibrary::TEXTURE_2D_ASSET); 
				action1->Rename("playButtonLoadTexture");
				AssetLibrary::_GetAssets().AddObject(action1);	 
			
				
					
				// ��������
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "playButton", true);// �������� ���������������� ��������
				action2->Rename("playButtonEnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action2);	 


				
				// ��������
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "playButton", "play");
				action3->Rename("playButtonChangeTexture"); 
				AssetLibrary::_GetAssets().AddObject(action3);	

				


				// ��������
				IAction* action5 = Trigger::_CreateTemplateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "#playButton", "#frameSize");
				action5->Rename("playButtonCreateQuad");
				AssetLibrary::_GetAssets().AddObject(action5);	

			
				
				// �������� 
				IAction* action6 = Trigger::_CreateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "playButton", Vector3D(-0.15f, -0.25f, 0));
				action6->Rename("playButtonReplacePosition");
				AssetLibrary::_GetAssets().AddObject(action6);	 
 
				
				playButtonLoader->SetEventExpr("runnerInit2"); 
				playButtonLoader->SaveToFile(); 
				 
				*/ 
			
				 
				AssetLibrary::_GetAssets().AddObject(playButtonLoader); 
			}

			 
 
			{
				// ��������� ������ Play...	
				Trigger* pickPlayButton1 = new Trigger();
				pickPlayButton1->LoadFromFile("pickPlayButton1");
				
				 
				/*
				
				pickPlayButton1->Rename("pickPlayButton1");
				pickPlayButton1->SetActionExpr("checkPlayButtonUnderCursor");	
				 

				
				// �������
				IEvent* event1 = Trigger::_CreateEvent(InputHandler::INPUT_EVENT_MESSAGE, "leftClick");// ���� ������
				event1->Rename("mouseLeftClick");
				AssetLibrary::_GetAssets().AddObject(event1);



				// ��������
				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "playButton", "playButtonUnderCursor", "", "");
				action1->Rename("checkPlayButtonUnderCursor");  
				AssetLibrary::_GetAssets().AddObject(action1);

				
				pickPlayButton1->SetEventExpr("mouseLeftClick"); 
				pickPlayButton1->SaveToFile();
				
				*/ 
				AssetLibrary::_GetAssets().AddObject(pickPlayButton1); 



				// ������� �� ���� �� ������...
				Trigger* pickPlayButton2 = new Trigger();
				pickPlayButton2->LoadFromFile("pickPlayButton2");
				
				
				/*
				pickPlayButton2->Rename("pickPlayButton2");
				pickPlayButton2->SetActionExpr("playAnim"); 
				

				// ��������
				IAction* action2 = Trigger::_CreateAction(AbstractAnimation::CHANGE_ANIMATION_STATE_MESSAGE, "run_right_source_anim", AbstractAnimation::PLAY);
				action2->Rename("playAnim");
				AssetLibrary::_GetAssets().AddObject(action2);
				
				pickPlayButton2->SetEventExpr("playButtonUnderCursor");
				pickPlayButton2->SaveToFile();
				
			 
			    */ 
				AssetLibrary::_GetAssets().AddObject(pickPlayButton2); 
 
			
	
				// ������� �� ���� �� ������...
				Trigger* pickPlayButton3 = new Trigger();
				pickPlayButton3->LoadFromFile("pickPlayButton3");
				
				/*
			
				pickPlayButton3->Rename("pickPlayButton3");
				pickPlayButton3->SetActionExpr("pickPlayButton"); 
				

				// �������
				IEvent* event2 = Trigger::_CreateEvent(AbstractAnimation::CHANGE_ANIMATION_STATE_MESSAGE, "run_right_source_anim", AbstractAnimation::PLAY);
				event2->SetOverlappingEnable(true);
				event2->Rename("isPlay");
				AssetLibrary::_GetAssets().AddObject(event2);

				
				
				// ��������
				IAction* action3 = Trigger::_CreateAction(IColor::CHANGE_COLOR_MESSAGE, "playButton", RGBA(RGBA::GREEN_COLOR));
				action3->Rename("pickPlayButton");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				pickPlayButton3->SetEventExpr("isPlay");
				pickPlayButton3->SaveToFile();
				
				*/ 
				 
				AssetLibrary::_GetAssets().AddObject(pickPlayButton3); 

			
		
				// ������� �� ���� �� ������...
				Trigger* pickPlayButton4 = new Trigger();
			    pickPlayButton4->LoadFromFile("pickPlayButton4");

				
				/*
				
				pickPlayButton4->Rename("pickPlayButton4");
				pickPlayButton4->SetActionExpr("unpickPlayButton"); 
				
					
				// ��������
				IAction* action4 = Trigger::_CreateAction(IColor::CHANGE_COLOR_MESSAGE, "playButton", RGBA(RGBA::WHITE_COLOR));
				action4->Rename("unpickPlayButton");
				AssetLibrary::_GetAssets().AddObject(action4);

				
				pickPlayButton4->SetEventExpr("~isPlay"); 
				pickPlayButton4->SaveToFile();
				
				*/ 
			 
			  
				AssetLibrary::_GetAssets().AddObject(pickPlayButton4);
			}
		 
		

	
			{
				// ������� ������ Pause...
				Object* pauseButton = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				pauseButton->Rename("pauseButton");
				AssetLibrary::_GetAssets().AddObject(pauseButton);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(pauseButton);
				scene->AddGraphicsObject(sprite); // �������� � ����������� �����
			}
			

			// ������ Pause...
			{
				Trigger* pauseButtonLoader = new Trigger();
				pauseButtonLoader->LoadFromFile("pauseButtonLoader");
				
				/*
				
				pauseButtonLoader->Rename("pauseButtonLoader");
				pauseButtonLoader->SetActionExpr("pauseButtonLoadTexture, pauseButtonEnableTexturedMaterial, pauseButtonChangeTextured, pauseButtonCreateQuad, pauseButtonReplacePosition");

				
				// ��������
				IAction* action1 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "pause", AssetLibrary::TEXTURE_2D_ASSET);
				action1->Rename("pauseButtonLoadTexture");
				AssetLibrary::_GetAssets().AddObject(action1);	
			
					
				// ��������
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "pauseButton", true);// �������� ���������������� ��������
				action2->Rename("pauseButtonEnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// ��������
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "pauseButton", "pause");
				action3->Rename("pauseButtonChangeTextured"); 
				AssetLibrary::_GetAssets().AddObject(action3);	

			
				// ��������
				IAction* action4 = Trigger::_CreateTemplateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "#pauseButton", "#frameSize");
				action4->Rename("pauseButtonCreateQuad"); 
				AssetLibrary::_GetAssets().AddObject(action4);	

			
				// �������� 
				IAction* action5 = Trigger::_CreateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "pauseButton", Vector3D(-0.02, -0.25f, 0));
				action5->Rename("pauseButtonReplacePosition");
				AssetLibrary::_GetAssets().AddObject(action5);	


				
				pauseButtonLoader->SetEventExpr("runnerInit2");
				pauseButtonLoader->SaveToFile();
				*/ 
				
				
				AssetLibrary::_GetAssets().AddObject(pauseButtonLoader); 
			}

			
			{ 
				// ��������� ������ Pause...	
				Trigger* pickPauseButton1 = new Trigger();
				pickPauseButton1->LoadFromFile("pickPauseButton1");
				
				
				/*
				
				pickPauseButton1->Rename("pickPauseButton1");
				pickPauseButton1->SetActionExpr("checkPauseButtonUnderCursor");	
				 
	
				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "pauseButton", "pauseButtonUnderCursor", "", ""); 
				action1->Rename("checkPauseButtonUnderCursor");  
				AssetLibrary::_GetAssets().AddObject(action1);

                
				
				pickPauseButton1->SetEventExpr("mouseLeftClick"); 
				pickPauseButton1->SaveToFile();
				 
				
			    */ 
			    
				AssetLibrary::_GetAssets().AddObject(pickPauseButton1); 



				// ������� �� ���� �� ������...
				Trigger* pickPauseButton2 = new Trigger();
				pickPauseButton2->LoadFromFile("pickPauseButton2");
				
			   
				/*
				
				pickPauseButton2->Rename("pickPauseButton2");
				pickPauseButton2->SetActionExpr("pauseAnim"); 
				

				// ��������
				IAction* action2 = Trigger::_CreateAction(AbstractAnimation::CHANGE_ANIMATION_STATE_MESSAGE, "run_right_source_anim", AbstractAnimation::PAUSE);
				action2->Rename("pauseAnim");
				AssetLibrary::_GetAssets().AddObject(action2);


				pickPauseButton2->SetEventExpr("pauseButtonUnderCursor");
				pickPauseButton2->SaveToFile();
				
                */ 
				AssetLibrary::_GetAssets().AddObject(pickPauseButton2); 


		
				// ������� �� ���� �� ������...
				Trigger* pickPauseButton3 = new Trigger();
				pickPauseButton3->LoadFromFile("pickPauseButton3");
				
				
				/*
				
				pickPauseButton3->Rename("pickPauseButton3");
				pickPauseButton3->SetActionExpr("pickPauseButton"); 
				

				IEvent* event2 = Trigger::_CreateEvent(AbstractAnimation::CHANGE_ANIMATION_STATE_MESSAGE, "run_right_source_anim", AbstractAnimation::PAUSE);
				event2->SetOverlappingEnable(true);
				event2->Rename("isPause");
				AssetLibrary::_GetAssets().AddObject(event2);

					
				// ��������
				IAction* action3 = Trigger::_CreateAction(IColor::CHANGE_COLOR_MESSAGE, "pauseButton", RGBA(RGBA::GREEN_COLOR));
				action3->Rename("pickPauseButton");
				AssetLibrary::_GetAssets().AddObject(action3);


				pickPauseButton3->SetEventExpr("isPause");
				pickPauseButton3->SaveToFile();
				
				*/ 
				  
				 
				AssetLibrary::_GetAssets().AddObject(pickPauseButton3); 


				// ������� �� ���� �� ������...
				Trigger* pickPauseButton4 = new Trigger();
				pickPauseButton4->LoadFromFile("pickPauseButton4");
				
				
				/*
			    
				pickPauseButton4->Rename("pickPauseButton4");
				pickPauseButton4->SetActionExpr("unpickPauseButton"); 
				
					
				// ��������
				IAction* action4 = Trigger::_CreateAction(IColor::CHANGE_COLOR_MESSAGE, "pauseButton", RGBA(RGBA::WHITE_COLOR));
				action4->Rename("unpickPauseButton");
				AssetLibrary::_GetAssets().AddObject(action4);


				pickPauseButton4->SetEventExpr("~isPause");
				pickPauseButton4->SaveToFile();
				*/ 
				
				  
				AssetLibrary::_GetAssets().AddObject(pickPauseButton4);
			}


			
			{
				// ������� ������ Stop...
				Object* stopButton = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				stopButton->Rename("stopButton");
				AssetLibrary::_GetAssets().AddObject(stopButton);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(stopButton);
				scene->AddGraphicsObject(sprite); // �������� � ����������� �����
			}

			// ������ Stop...
			{
				Trigger* stopButtonLoader = new Trigger();
				stopButtonLoader->LoadFromFile("stopButtonLoader");
				
				/*
				
				
				stopButtonLoader->Rename("stopButtonLoader");
				stopButtonLoader->SetActionExpr("stopButtonLoadTexture, stopButtonEnableTexturedMaterial, stopButtonChangeTextured, stopButtonCreateQuad, stopButtonReplacePosition");

				
				// ��������
				IAction* action1 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "stop", AssetLibrary::TEXTURE_2D_ASSET);
				action1->Rename("stopButtonLoadTexture");
				AssetLibrary::_GetAssets().AddObject(action1);	
			
					
				// ��������
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "stopButton", true);// �������� ���������������� ��������
				action2->Rename("stopButtonEnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// ��������
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "stopButton", "stop");
				action3->Rename("stopButtonChangeTextured"); 
				AssetLibrary::_GetAssets().AddObject(action3);	

			
				// ��������
				IAction* action4 = Trigger::_CreateTemplateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "#stopButton", "#frameSize");
				action4->Rename("stopButtonCreateQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	

			
				// �������� 
				IAction* action5 = Trigger::_CreateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "stopButton", Vector3D(0.11, -0.25f, 0));
				action5->Rename("stopButtonReplacePosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				stopButtonLoader->SetEventExpr("runnerInit2");
				stopButtonLoader->SaveToFile();
				*/ 
				
			   
				AssetLibrary::_GetAssets().AddObject(stopButtonLoader); 
			}

			

			{ 
				// ��������� ������ Stop...	
				Trigger* pickStopButton1 = new Trigger();
			    pickStopButton1->LoadFromFile("pickStopButton1");
				
			
				/*
				pickStopButton1->Rename("pickStopButton1");
				pickStopButton1->SetActionExpr("checkStopButtonUnderCursor");	
				 

						
				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "stopButton", "stopButtonUnderCursor", "", "");
				action1->Rename("checkStopButtonUnderCursor");  
				AssetLibrary::_GetAssets().AddObject(action1);


				
				pickStopButton1->SetEventExpr("mouseLeftClick"); 
				pickStopButton1->SaveToFile();
				*/ 
				
				
				AssetLibrary::_GetAssets().AddObject(pickStopButton1); 



				// ������� �� ���� �� ������...
				Trigger* pickStopButton2 = new Trigger();
				pickStopButton2->LoadFromFile("pickStopButton2");
				
				/*
				
				
				pickStopButton2->Rename("pickStopButton2");
				pickStopButton2->SetActionExpr("stopAnim"); 
				

				// ��������
				IAction* action2 = Trigger::_CreateAction(AbstractAnimation::CHANGE_ANIMATION_STATE_MESSAGE, "run_right_source_anim", AbstractAnimation::STOP);
				action2->Rename("stopAnim");
				AssetLibrary::_GetAssets().AddObject(action2);


				
				pickStopButton2->SetEventExpr("stopButtonUnderCursor");
				pickStopButton2->SaveToFile();
				
				*/ 
				
				AssetLibrary::_GetAssets().AddObject(pickStopButton2); 


		
				// ������� �� ���� �� ������...
				Trigger* pickStopButton3 = new Trigger();
				pickStopButton3->LoadFromFile("pickStopButton3");
				
		       
				/*
				
				pickStopButton3->Rename("pickStopButton3");
				pickStopButton3->SetActionExpr("pickStopButton"); 
				

				IEvent* event2 = Trigger::_CreateEvent(AbstractAnimation::CHANGE_ANIMATION_STATE_MESSAGE, "run_right_source_anim", AbstractAnimation::STOP);
				event2->SetOverlappingEnable(true);
				event2->Rename("isStop");
				AssetLibrary::_GetAssets().AddObject(event2);

					
				// ��������
				IAction* action3 = Trigger::_CreateAction(IColor::CHANGE_COLOR_MESSAGE, "stopButton", RGBA(RGBA::GREEN_COLOR));
				action3->Rename("pickStopButton");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				pickStopButton3->SetEventExpr("isStop");
				pickStopButton3->SaveToFile();
				 
				*/ 
				 
				AssetLibrary::_GetAssets().AddObject(pickStopButton3); 


				// ������� �� ���� �� ������...
				Trigger* pickStopButton4 = new Trigger();
				pickStopButton4->LoadFromFile("pickStopButton4");
				
				
				/*
				
				pickStopButton4->Rename("pickStopButton4");
				pickStopButton4->SetActionExpr("unpickStopButton"); 
				
					
				// ��������
				IAction* action4 = Trigger::_CreateAction(IColor::CHANGE_COLOR_MESSAGE, "stopButton", RGBA(RGBA::WHITE_COLOR));
				action4->Rename("unpickStopButton");
				AssetLibrary::_GetAssets().AddObject(action4);


				
				pickStopButton4->SetEventExpr("~isStop");
				pickStopButton4->SaveToFile();
				
				*/ 
				AssetLibrary::_GetAssets().AddObject(pickStopButton4);
			}
 
			 

			{
				// ������� ����...
				Object* quad = Object::_CreateObject(Object::_CreateObjectType(IColoredSprite::COLORED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				quad->Rename("quad");
				quad->SaveToFile();
			}
			
 
			
			{
				// �������� ���������� ��� �����
				Trigger* equality1 = new Trigger();
				equality1->LoadFromFile("equality1");
				

				/*
			
			
				equality1->Rename("equality1");
				equality1->SetActionExpr("createIntVariable, getAmountOfFrames, changeValue3, createBoolVariable, checkEquality");
	
				
				// �������
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "playButtonLoader");// ���� ������
				event1->Rename("playInit");
				AssetLibrary::_GetAssets().AddObject(event1);

	
				// �������
				IEvent* event2 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "pauseButtonLoader");// ���� ������
				event2->Rename("pauseInit");
				AssetLibrary::_GetAssets().AddObject(event2);


				// �������
				IEvent* event3 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "stopButtonLoader");// ���� ������
				event3->Rename("stopInit");
				AssetLibrary::_GetAssets().AddObject(event3);


				// ��������
				IAction* action1 = Trigger::_CreateAction(IVariable::CREATE_VARIABLE_MESSAGE, "frameCount", Variable<int32>::INTEGER_TYPE, 0);
				action1->Rename("createIntVariable");
				AssetLibrary::_GetAssets().AddObject(action1);	
	

				// ��������
				IAction* action2 = Trigger::_CreateAction(AbstractAnimation::GET_AMOUNT_OF_FRAMES_MESSAGE, "run_right_source_anim", "frameCount");
				action2->Rename("getAmountOfFrames");
				AssetLibrary::_GetAssets().AddObject(action2);	 


			
				// ��������
				IAction* action3 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "frameSize", "frameSize / 3.0", IVariable::_GetValueType<Vector2D>());
				action3->Rename("changeValue3");  
				AssetLibrary::_GetAssets().AddObject(action3);	


				
				// ��������
				IAction* action4 = Trigger::_CreateAction(IVariable::CREATE_VARIABLE_MESSAGE, "loop", Variable<bool>::BOOLEAN_TYPE, false);
				action4->Rename("createBoolVariable");
				AssetLibrary::_GetAssets().AddObject(action4);	


				// ��������
				IAction* action5 = Trigger::_CreateAction(IVariable::CHECK_EQUALITY_MESSAGE, "loop", "~(currentFrameIndex ? frameCount)");
				action5->Rename("checkEquality");
				AssetLibrary::_GetAssets().AddObject(action5);
 

				equality1->SetEventExpr("playInit & pauseInit & stopInit"); 
				equality1->SaveToFile();
				
				*/ 
			
			    
				AssetLibrary::_GetAssets().AddObject(equality1);	
			}
 
		
					

			{ // ������� ��� �������� � �������, ������� ����� ������������ ������...
				
				// �������
				IEvent* event1 = Trigger::_CreateTemplateEvent(AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE, "#run_right_source_anim", "#frameIndex + currentFrameIndex");
				event1->SetOverlappingEnable(true);
				event1->Rename("currentFrame");
				event1->SaveToFile();
 

				// �������� 
				IAction* action1 = Trigger::_CreateTemplateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "#frame + currentFrameIndex", "#frameUnderCursor + currentFrameIndex", "", "");
				action1->Rename("checkFrameUnderCursor");   
				action1->SaveToFile();


				// ��������
				IAction* action2 = Trigger::_CreateTemplateAction(IColor::CHANGE_COLOR_MESSAGE, "#frame + currentFrameIndex", "#activatedColorVar");
				action2->Rename("pickFrame");  
				action2->SaveToFile();

				

				// ��������
				IAction* action3 = Trigger::_CreateTemplateAction(IColor::CHANGE_COLOR_MESSAGE, "#frame + currentFrameIndex", "#deactivatedColorVar");
				action3->Rename("unpickFrame");
				action3->SaveToFile();

			

				// ��������
				IAction* action4 = Trigger::_CreateTemplateAction(AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE, "#run_right_source_anim", "#frameIndex + currentFrameIndex");
				action4->Rename("setFrame");  
				action4->SaveToFile();
			}

			

			{// ����� ������� ����� ��������...
				Trigger* trigger = new Trigger(); 
				trigger->LoadFromFile("trigger");
				
			
				/*
				trigger->Rename("trigger"); 
				trigger->SetActionExpr("setFrameZero");
		
					
				// ������� 
				IEvent* event1 = Trigger::_CreateEvent(AssetLibrary::CREATE_OBJECT_MESSAGE, "frame1");  
				event1->Rename("frame1Created");
				AssetLibrary::_GetAssets().AddObject(event1); 
				

				// �������� 
				IAction* action = Trigger::_CreateAction(AbstractAnimation::CHANGE_ANIMATION_FRAME_MESSAGE, "run_right_source_anim", 0);
				action->Rename("setFrameZero"); 
				AssetLibrary::_GetAssets().AddObject(action);

				
				trigger->SetEventExpr("frame1Created"); 
				trigger->SaveToFile();
				
				*/

				AssetLibrary::_GetAssets().AddObject(trigger); 
			}
			

			

		
			{ // ����... 
				Trigger* equality2 = new Trigger();
				equality2->LoadFromFile("equality2"); 

				/*
				
				equality2->Rename("equality2"); 
				equality2->SetActionExpr("frameLoad, frameCreateQuad, framePositionUpdate, frameReplacePosition, frameChangeUnselectColor, addFrameToGraphicsScene, changeFrameName, frameRename, createFrameVariable, changeFrameVariableName, renameFrameVariableName, createTrigger1Name, createTrigger1EventExprName, createTrigger1ActionExprName, createTrigger1, loadAction1, createAction1Name, renameAction1, createTrigger2Name, createTrigger2EventExprName, createTrigger2ActionExprName, createTrigger2, createFrameIndexVariable, setFrameIndexValue, createFrameIndexName, renameFrameIndex, loadEvent2, createEvent2Name, renameEvent2, loadAction2, createAction2Name, renameAction2, createTrigger3Name, createTrigger3EventExprName, createTrigger3ActionExprName, createTrigger3, loadEvent3, createEvent3Name, renameEvent3, createTrigger4Name, createTrigger4EventExprName, createTrigger4ActionExprName, createTrigger4, loadAction4, createAction4Name, renameAction4, shorten, checkEquality");
 


				// ��������
				IAction* action1 = Trigger::_CreateTemplateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "#quadVar", "#objectTypeVar");
				action1->Rename("frameLoad"); 
				AssetLibrary::_GetAssets().AddObject(action1);

		
				// ��������
				IAction* action2 = Trigger::_CreateTemplateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "#quad", "#frameSize");
				action2->Rename("frameCreateQuad"); 
				AssetLibrary::_GetAssets().AddObject(action2);	


				// �������� 
				IAction* action3 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#framePosition", "framePosition + offset", Variable<Vector3D>::VECTOR_3D_TYPE);
				action3->Rename("framePositionUpdate");  
				AssetLibrary::_GetAssets().AddObject(action3);


				// �������� 
				IAction* action4 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#quad", "#framePosition");
				action4->Rename("frameReplacePosition");
				AssetLibrary::_GetAssets().AddObject(action4);

		
				// �������� 
				IAction* action5 = Trigger::_CreateTemplateAction(IColor::CHANGE_COLOR_MESSAGE, "#quad", "#deactivatedColorVar");
				action5->Rename("frameChangeUnselectColor"); 
				AssetLibrary::_GetAssets().AddObject(action5);	


				// �������� 
				IAction* action6 = Trigger::_CreateTemplateAction(GraphicsScene::MOVE_TO_GRAPHICS_SCENE_MESSAGE, "#scene", "#quad");
				action6->Rename("addFrameToGraphicsScene"); 
				AssetLibrary::_GetAssets().AddObject(action6);	 
		

				
				// ��������
				IAction* action7 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#newName", "#frame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action7->Rename("changeFrameName");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// ��������
				IAction* action8 = Trigger::_CreateTemplateAction(Object::RENAME_OBJECT_MESSAGE, "#quad", "#newName");
				action8->Rename("frameRename");
				AssetLibrary::_GetAssets().AddObject(action8);	

				
				
				// ��������
				IAction* action9 = Trigger::_CreateTemplateAction(IVariable::CREATE_VARIABLE_MESSAGE, "#frameUnderCursor", Variable<bool>::BOOLEAN_TYPE, false);
				action9->Rename("createFrameVariable");
				AssetLibrary::_GetAssets().AddObject(action9);


				// ��������
				IAction* action10 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#newName", "#frameUnderCursor + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action10->Rename("changeFrameVariableName");
				AssetLibrary::_GetAssets().AddObject(action10);	


				// ��������
				IAction* action11 = Trigger::_CreateTemplateAction(Object::RENAME_OBJECT_MESSAGE, "#frameUnderCursor", "#newName");
				action11->Rename("renameFrameVariableName");
				AssetLibrary::_GetAssets().AddObject(action11);

				
				// createTrigger1Name, createTrigger1EventExprName, createTrigger1ActionExprName, createTrigger1, loadAction1, createAction1Name, renameAction1
				
				// ��������
				IAction* action12 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerName", "#frameTrigger1_ + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action12->Rename("createTrigger1Name");
				AssetLibrary::_GetAssets().AddObject(action12);	

				
				// ��������
				IAction* action13 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerEventExpr", "#mouseLeftClick", Variable<StringANSI>::STRING_TYPE);
				action13->Rename("createTrigger1EventExprName");
				AssetLibrary::_GetAssets().AddObject(action13);	


				// ��������
				IAction* action14 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerActionExpr", "#checkFrameUnderCursor + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action14->Rename("createTrigger1ActionExprName"); 
				AssetLibrary::_GetAssets().AddObject(action14);	


				// ��������
				IAction* action15 = Trigger::_CreateTemplateAction(Trigger::CREATE_TRIGGER_MESSAGE, "#triggerName", "#triggerEventExpr", "#triggerActionExpr");
				action15->Rename("createTrigger1");
				AssetLibrary::_GetAssets().AddObject(action15);

	
				// ��������
				IAction* action16 = Trigger::_CreateTemplateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "#checkFrameUnderCursorVar", "#actionTypeVar");  
				action16->Rename("loadAction1");
				AssetLibrary::_GetAssets().AddObject(action16);

	
				// ��������
				IAction* action17 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#newName", "#checkFrameUnderCursor + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action17->Rename("createAction1Name");
				AssetLibrary::_GetAssets().AddObject(action17);	


				// ��������
				IAction* action18 = Trigger::_CreateTemplateAction(Object::RENAME_OBJECT_MESSAGE, "#checkFrameUnderCursor", "#newName");
				action18->Rename("renameAction1");
				AssetLibrary::_GetAssets().AddObject(action18);

				
				// createTrigger2Name, createTrigger2EventExprName, createTrigger2ActionExprName, createTrigger2, loadEvent2, createEvent2Name, renameEvent2, loadAction2, createAction2Name, renameAction2
				
				// ��������
				IAction* action19 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerName", "#frameTrigger2_ + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action19->Rename("createTrigger2Name");
				AssetLibrary::_GetAssets().AddObject(action19);	 


				// �������� 
				IAction* action20 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerEventExpr", "#currentFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE); 
				action20->Rename("createTrigger2EventExprName");  
				AssetLibrary::_GetAssets().AddObject(action20);	 


				// ��������
				IAction* action21 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerActionExpr", "#pickFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE); 
				action21->Rename("createTrigger2ActionExprName"); 
				AssetLibrary::_GetAssets().AddObject(action21);	


				// ��������
				IAction* action22 = Trigger::_CreateTemplateAction(Trigger::CREATE_TRIGGER_MESSAGE, "#triggerName", "#triggerEventExpr", "#triggerActionExpr"); 
				action22->Rename("createTrigger2");  
				AssetLibrary::_GetAssets().AddObject(action22); 


				// ��������
				IAction* action23 = Trigger::_CreateTemplateAction(IVariable::CREATE_VARIABLE_MESSAGE, "#frameIndex", Variable<int32>::INTEGER_TYPE, 0);
				action23->Rename("createFrameIndexVariable");
				AssetLibrary::_GetAssets().AddObject(action23);	


				// ��������
				IAction* action24 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#frameIndex", "currentFrameIndex", Variable<int32>::INTEGER_TYPE);
				action24->Rename("setFrameIndexValue");
				AssetLibrary::_GetAssets().AddObject(action24);

					
				// ��������
				IAction* action25 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#newName", "#frameIndex + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action25->Rename("createFrameIndexName");
				AssetLibrary::_GetAssets().AddObject(action25);	


				// ��������
				IAction* action26 = Trigger::_CreateTemplateAction(Object::RENAME_OBJECT_MESSAGE, "#frameIndex", "#newName");
				action26->Rename("renameFrameIndex");
				AssetLibrary::_GetAssets().AddObject(action26);


				// ��������
				IAction* action27 = Trigger::_CreateTemplateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "#currentFrameVar", "#eventTypeVar");   
				action27->Rename("loadEvent2"); 
				AssetLibrary::_GetAssets().AddObject(action27);

	
				// ��������
				IAction* action28 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#newName", "#currentFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action28->Rename("createEvent2Name");
				AssetLibrary::_GetAssets().AddObject(action28);	


				// ��������
				IAction* action29 = Trigger::_CreateTemplateAction(Object::RENAME_OBJECT_MESSAGE, "#currentFrame", "#newName");
				action29->Rename("renameEvent2");  
				AssetLibrary::_GetAssets().AddObject(action29);

				
				// �������� 
				IAction* action30 = Trigger::_CreateTemplateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "#pickFrameVar", "#actionTypeVar");  
				action30->Rename("loadAction2");
				AssetLibrary::_GetAssets().AddObject(action30);

	
				// ��������
				IAction* action31 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#newName", "#pickFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action31->Rename("createAction2Name");
				AssetLibrary::_GetAssets().AddObject(action31);	


				// ��������
				IAction* action32 = Trigger::_CreateTemplateAction(Object::RENAME_OBJECT_MESSAGE, "#pickFrame", "#newName");
				action32->Rename("renameAction2");
				AssetLibrary::_GetAssets().AddObject(action32);


		

				// createTrigger3Name, createTrigger3EventExprName, createTrigger3ActionExprName, createTrigger3, loadEvent3, createEvent3Name, renameEvent3

				// ��������
				IAction* action33 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerName", "#frameTrigger3_ + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action33->Rename("createTrigger3Name");
				AssetLibrary::_GetAssets().AddObject(action33);	

				
				// ��������
				IAction* action34 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerEventExpr", "NOT + #currentFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE); 
				action34->Rename("createTrigger3EventExprName");  
				AssetLibrary::_GetAssets().AddObject(action34);	 


				// ��������
				IAction* action35 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerActionExpr", "#unpickFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE); 
				action35->Rename("createTrigger3ActionExprName"); 
				AssetLibrary::_GetAssets().AddObject(action35);	


				// ��������
				IAction* action36 = Trigger::_CreateTemplateAction(Trigger::CREATE_TRIGGER_MESSAGE, "#triggerName", "#triggerEventExpr", "#triggerActionExpr"); 
				action36->Rename("createTrigger3");  
				AssetLibrary::_GetAssets().AddObject(action36); 
				
				
				// �������� 
				IAction* action37 = Trigger::_CreateTemplateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "#unpickFrameVar", "#actionTypeVar");  
				action37->Rename("loadEvent3");
				AssetLibrary::_GetAssets().AddObject(action37);

	
				// ��������
				IAction* action38 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#newName", "#unpickFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action38->Rename("createEvent3Name");
				AssetLibrary::_GetAssets().AddObject(action38);	


				// ��������
				IAction* action39 = Trigger::_CreateTemplateAction(Object::RENAME_OBJECT_MESSAGE, "#unpickFrame", "#newName");
				action39->Rename("renameEvent3");
				AssetLibrary::_GetAssets().AddObject(action39); 



				// createTrigger4Name, createTrigger4EventExprName, createTrigger4ActionExprName, createTrigger4, loadAction4, createAction4Name, renameAction4
 

				// ��������
				IAction* action40 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerName", "#frameTrigger4_ + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action40->Rename("createTrigger4Name");
				AssetLibrary::_GetAssets().AddObject(action40);	 


				// ��������
				IAction* action41 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerEventExpr", "#frameUnderCursor + currentFrameIndex", Variable<StringANSI>::STRING_TYPE); 
				action41->Rename("createTrigger4EventExprName");  
				AssetLibrary::_GetAssets().AddObject(action41);	 


				// ��������
				IAction* action42 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#triggerActionExpr", "#setFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE); 
				action42->Rename("createTrigger4ActionExprName"); 
				AssetLibrary::_GetAssets().AddObject(action42);	


				// ��������
				IAction* action43 = Trigger::_CreateTemplateAction(Trigger::CREATE_TRIGGER_MESSAGE, "#triggerName", "#triggerEventExpr", "#triggerActionExpr"); 
				action43->Rename("createTrigger4"); 
				AssetLibrary::_GetAssets().AddObject(action43); 
				
	
				// �������� 
				IAction* action44 = Trigger::_CreateTemplateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "#setFrameVar", "#actionTypeVar");
				action44->Rename("loadAction4"); 
				AssetLibrary::_GetAssets().AddObject(action44);

	

				// ��������
				IAction* action45 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#newName", "#setFrame + currentFrameIndex", Variable<StringANSI>::STRING_TYPE);
				action45->Rename("createAction4Name");
				AssetLibrary::_GetAssets().AddObject(action45);	


				// ��������
				IAction* action46 = Trigger::_CreateTemplateAction(Object::RENAME_OBJECT_MESSAGE, "#setFrame", "#newName");
				action46->Rename("renameAction4"); 
				AssetLibrary::_GetAssets().AddObject(action46);
				


				// ��������
				IAction* action47 = Trigger::_CreateTemplateAction(IVariable::CHANGE_VALUE_MESSAGE, "#currentFrameIndex", "currentFrameIndex + 1", Variable<int32>::INTEGER_TYPE);
				action47->Rename("shorten");
				AssetLibrary::_GetAssets().AddObject(action47);

				

				equality2->SetEventExpr("loop"); 
				equality2->SaveToFile();
				
				*/ 
			
				AssetLibrary::_GetAssets().AddObject(equality2); 
			}

			
			
	
			
		

			
			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			
			MoveToCenter();// ����������� ���� � ����� �����
			Mouse::_MoveCursorToCenter(this);// ����������� ������ � ����� ���������� ������� ����	
			//Mouse::_ShowCursor(false);// ������ ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}

	

	// ������� �����
	public: virtual void Update(float _delta)
	{
		scene->Update(_delta);
		Trigger::_Execute();
	}



	// ������� �������
	public: virtual void Draw(float _delta)
	{
		ClearScene();// �������� �����


		
		/*
		camera->BeginScreen2D();// ����� ���������� �������


		// ����� ������...
		int32 i = 0;
		*/ 


	

		/*
		Object* obj = AssetLibrary::_GetAssets().FindObject("john_gregory_hello_reflex1");
		if(obj)
		{
			Trigger* tr = dynamic_cast<Trigger*>(obj);
			font2d->DrawScreenText(Vector3D(10, i+=20, -1), "actions: %d", tr->GetActions().Size()); 
		
			font2d->DrawScreenText(Vector3D(10, i+=20, -1), "defined events: %d", tr->GetAmountOfDefinedVariables());
			font2d->DrawScreenText(Vector3D(10, i+=20, -1), "undefined events: %d", tr->GetAmountOfUndefinedVariables());

			font2d->DrawScreenText(Vector3D(10, i+=20, -1), "expr: %s", tr->GetBooleanExpr().c_str());

		}

		*/ 

		//userFont->DrawScreenText(Vector3D(chat., i+=20, -1), "act1.contacts: %d", act1->GetAmountOfContacts());
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "act1.messages: %d", act1->AmountOfMessages());
		


		camera->BeginPerspective3D();// ����� ���������� �������

		 
		scene->Draw();// ���������� ������� �����
	}


	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Mouse().ClearEventList();// �������� ����� �������
		return _event;
	}


	// ��������� ������� ����������
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		if(_event->type == Keyboard::KEY_DOWN)
		{
			if(_event->key == Keyboard::F1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event->key == Keyboard::F2)
			{
				SetWindowStyle(Window::WND_RESIZING);
			}
			else if(_event->key == Keyboard::ESC)
			{
				Destroy();// ���������� ����
			} 
		}

		Keyboard().ClearEventList();// �������� ����� �������
		return _event;
	}
};



// ����� ����� � ���������
// ���������� � ������ ������� WINAPI, ���������, ��� ��������� � ������� WinMain ���������� ����� �������
int WINAPI WinMain(HINSTANCE hInstance,// ���������� (�������������), ������������ �������� ��� ������� ����������
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// �������� ������...
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// ����� ���� 
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "Trigger.example_05";

	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
