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
#include <Engine/Output/Graphics/Shape/Mesh/ColoredMesh/ColoredMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Shape/Mesh/TexturedMesh/TexturedMesh.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>




// ���������������� ����������
class Application: public Engine,// ������
         	       public MessageHandler<int32>::Receiver// ���������� ���������
{
	// �������������� �������� ��������� �������
	public: enum
	{
		NO_SELECT,// ��� ���������
		UNITS_PER_SECOND,
		FIXED_TIME,
		PARABOLA,
		DAMPING, 
		MAGNETIC,
	};

	// DATA
	private: Camera3D* camera, *camera2d;// ������
    private: Font2D *font;// �����
	private: Scene* scene;// �����

	private: FloatLerp* unitsPerSecondLerp, *fixedTimeLerp, *parabolaLerp, *dampingLerp, *magneticLerp;// ������������ float-�����
	private: Graph* graph;// ������ �������
	private: bool rightPressed;// ���� true, �� ������ ������ ���� ������
	private: int32 index;// ������ �������� ����������� ����
    private: FloatLerp* selected;// ���������� �������, ���������� � ������� ����� ����������
	//private: float accum[5];// �����, ������� ���� �������� ����� ������������ ����� �������; ��� ������ ��������������� �������� �������� ��������������
    private: TexturedMesh* clenchedHandCursor;// ������ "������ ����"
	private: TexturedMesh* unclenchedHandCursor;// ������ "�������� ����"
	private: TexturedMesh* arrowCursor;// ������ "�������"
	private: Graph::Function *unitsPerSecond, *fixedTime, *parabola, *damping, *magnetic;// ����������� �������, �������� ������������ ������� �� �������
	private: int32 selectedMarker;// ������������� ����������� ������� �������
	private: bool startMarkerMoving;// ���� true, �� ������ ������������ � ������� ����
	private: Vector3 startMarkerPosition;// ��������� ������� ������� �� ��������� ������� � ������ ����������� ������� ������� � ������� ����


    // METHODS
    // ���������������� �����������
	public: Application(void): camera(NIL), font(NIL), scene(NIL), unitsPerSecondLerp(NIL), fixedTimeLerp(NIL), parabolaLerp(NIL), dampingLerp(NIL), magneticLerp(NIL), graph(NIL), rightPressed(false), index(0), selected(NIL), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), arrowCursor(NIL), unitsPerSecond(NIL), fixedTime(NIL), parabola(NIL), damping(NIL), magnetic(NIL), selectedMarker(NO_SELECT), startMarkerMoving(false)
	{
		/*
		for(int32 i = 0; i < 5; i++)
		{
			accum[i] = 0.0f;
		}
		*/ 
	}

			
	// ����������
	public: virtual ~Application(void)
	{
		delete graph;

		delete unitsPerSecondLerp; 
		delete fixedTimeLerp; 
		delete parabolaLerp; 
		delete dampingLerp;
		delete magneticLerp;

		delete clenchedHandCursor;
		delete unclenchedHandCursor;
		delete arrowCursor;

		delete camera;
		delete camera2d;
		delete font;
		delete scene;
	} 



	// ������� ����������
	public: bool Create(Engine::Define _define)
	{ 
		// ������� ������...
		if(Engine::Create(_define))
		{
			// �����...
			{
				scene = new Scene();
				scene->Pick();
			}

			// �����...
			{
				Font2D::_CheckSupportedFonts(this);// ������������� ������ �������������� �������

				Font2D::Define def;
				def.fontName = "Arial";// ��� ������
				def.fontSize = 16;// ������ ������
				def.fontWeight = 600;
				def.color = RGBA(RGBA::GRAY_COLOR);// ���� ������

				font = new Font2D();
				font->Create(def);
				font->Pick();// ������� �����
			}


			// ������...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;
				def.rotationEnable = false;// ��������� �������� ������
				def.yawClampEnable = true;
				def.pitchClampEnable = true;
				def.zoomSpeed = 3.0f;
				def.zoomEnable = true;

				def.pitchMax = Angle::_DegreesToRadians(40);
				def.pitchMin = Angle::_DegreesToRadians(-40);
				def.yawMax = Angle::_DegreesToRadians(40);
				def.yawMin = Angle::_DegreesToRadians(-40);



				camera = new Camera3D();
				camera->Set(def);

				// ������ ����������� � ������������ ��� ������
				camera->SetNearZoom(camera->GetNearZoom() * 100);
				camera->SetFarZoom(camera->GetFarZoom() * 100);
				camera->SetDistanceToViewingPoint(140.0f); 

				camera->Pick(); // ������� ������
				scene->BindCamera(camera);
			}
		
			
			// ������ "�������� ����"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("unclenched");

				unclenchedHandCursor = new TexturedMesh();
				unclenchedHandCursor->CreateQuad2D(Quad(Vector3(), Vector2(24, 29)));
				unclenchedHandCursor->BindAnimation(anim);
				
				unclenchedHandCursor->ConvertTo2DObject();
				unclenchedHandCursor->SetShowEnable(true);// �������� �����������
				//scene->AddGraphicsObject(unclenchedHandCursor);// �������� � ����������� �����
			}


			// ������ "������ ����"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("clenched");

				clenchedHandCursor = new TexturedMesh();
				clenchedHandCursor->CreateQuad2D(Quad(Vector3(), Vector2(22, 24)));
				clenchedHandCursor->BindAnimation(anim);

				clenchedHandCursor->SetShowEnable(true);// �������� �����������
				clenchedHandCursor->ConvertTo2DObject();
			}

			
			// ������ "�������"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("cursor");

				arrowCursor = new TexturedMesh();
				arrowCursor->CreateQuad2D(Quad(Vector3(14/2, 24/2, 0), Vector2(14, 24)));
				arrowCursor->BindAnimation(anim);

				arrowCursor->SetShowEnable(true);// �������� �����������
				arrowCursor->ConvertTo2DObject();
				Engine::_PickCursor(arrowCursor);// �������� ������
			}

			// ������ ��� ���������� �������...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				def.pitchClampEnable = false;// ��������� ���������� �������� ������ ��� X
				
				camera2d = new Camera3D();
				camera2d->Set(def);
			}


			unitsPerSecondLerp = new FloatLerp();
			unitsPerSecondLerp->Rename("UNITS_PER_SECOND");// ������ ��� 
			unitsPerSecondLerp->SetLerpMode(FloatLerp::UNITS_PER_SECOND);//������� � �������
			unitsPerSecondLerp->SetPlayingSpeed(0.7f);// �������� �������� � ��������/�������
			unitsPerSecondLerp->SetTarget(6.0f);// ���������� �� 0 �� 5 ����� �������� �� 2 �������
			unitsPerSecondLerp->Play();
			scene->AddLogicObject(unitsPerSecondLerp);


			fixedTimeLerp = new FloatLerp();
			fixedTimeLerp->Rename("FIXED_TIME");// ������ ��� 
			fixedTimeLerp->SetLerpMode(FloatLerp::FIXED_TIME);//������� � �������
			fixedTimeLerp->SetPlayingSpeed(7);// �������� �������� � ��������
			fixedTimeLerp->SetTarget(15.0f);// ���������� �� ����
			fixedTimeLerp->Play();
			scene->AddLogicObject(fixedTimeLerp);


			parabolaLerp = new FloatLerp();
			parabolaLerp->Rename("PARABOLA");// ������ ��� 
			parabolaLerp->SetLerpMode(FloatLerp::PARABOLA);//������� � �������
			parabolaLerp->SetPlayingSpeed(0.1f);// �������� �������� 
			parabolaLerp->SetTarget(10.0f);// ���������� �� ����
			parabolaLerp->Play();
			scene->AddLogicObject(parabolaLerp);

			//SetParabolaPlayingDurationInSec(0.1f);



			dampingLerp = new FloatLerp();
			dampingLerp->Rename("DAMPING");// ������ ��� 
			dampingLerp->SetLerpMode(FloatLerp::DAMPING);
			dampingLerp->SetPlayingSpeed(0.5f);// �������� �������� 
			dampingLerp->SetTarget(10.0f);// ���������� �� ����
			dampingLerp->Play();
			scene->AddLogicObject(dampingLerp);

			//SetDampingPlayingDurationInSec(0.1f);

	
			magneticLerp = new FloatLerp();
			magneticLerp->Rename("MAGNETIC");// ������ ��� 
			magneticLerp->SetLerpMode(FloatLerp::MAGNETIC);
			magneticLerp->SetPlayingSpeed(1.1f);// �������� �������� 
			magneticLerp->SetTarget(00.0f);// ���������� �� ����
			magneticLerp->Play();
			scene->AddLogicObject(magneticLerp);

			///SetMagneticPlayingDurationInSec(0.3f);


			// ������ �������
			{
				graph = new Graph(Vector2(100.0f, 100.0f), 16, 16);
				//graph->SetColor(RGBA(RGBA::GRAY_COLOR));// ���� �������
				//graph->SetXColor(RGBA(RGBA::RED_COLOR));// ���� ������� ��� ��� X
				//graph->SetYColor(RGBA(RGBA::GREEN_COLOR));// ���� ������� ��� ��� Y
				graph->SetScaleX(10.0f);
				//graph->SetScaleY(2.0f);
				//graph->SetCurrentValueShowEnable(false);
			    graph->SetXRangeShowEnable(true);
				graph->SetYRangeShowEnable(true);
				graph->SetXAxisName("Time");
				graph->SetYAxisName("Value");
				
				unitsPerSecond = graph->CreateFunction(unitsPerSecondLerp->GetName());
				//unitsPerSecond->SetColor(RGBA(55, 111, 194, 255));
				unitsPerSecond->SetXMarkerColor(RGBA(55, 111, 194, 255) + RGBA(0, 0, 5, 0));
				unitsPerSecond->SetYMarkerColor(RGBA(55, 111, 194, 255) + RGBA(0, 5, 0, 0));
				unitsPerSecond->SetShowEnable(false);
				unitsPerSecond->SetXMarkerWidth(5.0f);
				unitsPerSecond->SetYMarkerWidth(5.0f);
				
				
				// �������� ������������ � �������� ��������
				graph->GetMarkList().AddToTail(Graph::Mark(unitsPerSecondLerp->GetTarget() / unitsPerSecondLerp->GetPlayingSpeed(), Graph::Mark::AXIS_X, RGBA(55, 111, 194, 255)));
				
				SelectFunction(FloatLerp::UNITS_PER_SECOND);// ������ ������� �������
				

				fixedTime = graph->CreateFunction(fixedTimeLerp->GetName());
				fixedTime->SetColor(RGBA(118, 185, 0, 255));
				fixedTime->SetXMarkerColor(RGBA(118, 185, 0, 255) + RGBA(0, 0, 5, 0));
				fixedTime->SetYMarkerColor(RGBA(118, 185, 0, 255) + RGBA(0, 5, 0, 0));
				fixedTime->SetShowEnable(false);
				fixedTime->SetXMarkerWidth(5.0f);
				fixedTime->SetYMarkerWidth(5.0f);
				


				parabola = graph->CreateFunction(parabolaLerp->GetName());
				parabola->SetColor(RGBA(163, 21, 21, 255));
				parabola->SetXMarkerColor(RGBA(163, 21, 21, 255) + RGBA(0, 0, 5, 0));
				parabola->SetYMarkerColor(RGBA(163, 21, 21, 255) + RGBA(0, 5, 0, 0));
				parabola->SetShowEnable(false);
				parabola->SetXMarkerWidth(5.0f);
				parabola->SetYMarkerWidth(5.0f);
				
				

				damping = graph->CreateFunction(dampingLerp->GetName());
				damping->SetColor(RGBA(230, 40, 230, 255));
				damping->SetXMarkerColor(RGBA(230, 40, 230, 255) + RGBA(0, 0, 5, 0));
				damping->SetYMarkerColor(RGBA(230, 40, 230, 255) + RGBA(0, 5, 0, 0));
				damping->SetShowEnable(false);
				damping->SetXMarkerWidth(5.0f);
				damping->SetYMarkerWidth(5.0f);
				
				
					
				magnetic = graph->CreateFunction(magneticLerp->GetName());
				magnetic->SetColor(RGBA(247, 170, 0, 255));
				magnetic->SetXMarkerColor(RGBA(247, 170, 0, 255) + RGBA(0, 0, 5, 0));
				magnetic->SetYMarkerColor(RGBA(247, 170, 0, 255) + RGBA(0, 5, 0, 0));
				magnetic->SetShowEnable(false);
				magnetic->SetXMarkerWidth(5.0f);
				magnetic->SetYMarkerWidth(5.0f);
				
				
				scene->AddGraphicsObject(graph);
			}

			

			Mouse::_SetShowCursor(false);// ������ ��������� ������

			// ���� ����...
			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));
			
			MoveToCenter();// ����������� ���� � ����� ������
			return true;// ������� �����
		}
		// ����� ������� �������
		return false;
	}


	// �������� ������� � �������� _index � �������� �������
	public: void SelectFunction(int32 _index)
	{
		if(_index >= 0 && _index < 5)
		{
			if(selected)
			{
				graph->FindFunction(selected->GetName())->SetShowEnable(false);// ������ 
			}

			index = _index;

			if(index == FloatLerp::UNITS_PER_SECOND)
			{
				selected = unitsPerSecondLerp;
			}
			else if(index == FloatLerp::PARABOLA)
			{
				selected = fixedTimeLerp;
			}
			else if(index == FloatLerp::FIXED_TIME)
			{
				selected = parabolaLerp;
			}
			else if(index == FloatLerp::DAMPING)
			{
				selected = dampingLerp;
			}
			else if(index == FloatLerp::MAGNETIC)
			{
				selected = magneticLerp;
			}
		
			graph->FindFunction(selected->GetName())->SetShowEnable(true);// ��������

							
			// �������� ������� ������� �� ��� �������....
			if(selected->GetLerpMode() == FloatLerp::PARABOLA)
			{
				graph->GetMarkList()[0]->key = Graph::Mark(GetParabolaPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());	 
			}
			if(selected->GetLerpMode() == FloatLerp::MAGNETIC)
			{
				graph->GetMarkList()[0]->key = Graph::Mark(GetMagneticPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());	 
			}
			if(selected->GetLerpMode() == FloatLerp::DAMPING)
			{
				graph->GetMarkList()[0]->key = Graph::Mark(GetDampingPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());	 
			}
			else
			{
				graph->GetMarkList()[0]->key = Graph::Mark(selected->GetPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());	 
			}
		}
	}


	// ������� ����������
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		// �������� �������� �������...
		if(unitsPerSecondLerp->IsPlaying())
		{
			//accum[0] += unitsPerSecondLerp->GetDelta();
			graph->AddVertex(unitsPerSecondLerp->GetName(), Vector2(_delta, unitsPerSecondLerp->GetCurrent()));
		}
		
		if(fixedTimeLerp->IsPlaying())
		{
			//accum[1] += fixedTimeLerp->GetDelta();
			graph->AddVertex(fixedTimeLerp->GetName(), Vector2(_delta, fixedTimeLerp->GetCurrent()));
		}

		if(parabolaLerp->IsPlaying())
		{
			//accum[2] += parabolaLerp->GetDelta();
			graph->AddVertex(parabolaLerp->GetName(), Vector2(_delta, parabolaLerp->GetCurrent()));
		}

		if(dampingLerp->IsPlaying())
		{
			//accum[3] += dampingLerp->GetDelta();
			graph->AddVertex(dampingLerp->GetName(), Vector2(_delta, dampingLerp->GetCurrent()));
		}

		if(magneticLerp->IsPlaying())
		{
			//accum[4] += magneticLerp->GetDelta();
			graph->AddVertex(magneticLerp->GetName(), Vector2(_delta, magneticLerp->GetCurrent()));
		}

		CheckCollisionWithMarkers();// ��������� ������� ������� �������� � ������� ����
	}



	// ���������� ��������� ������������� ������ �����������
	public: StringANSI LerpModeToString(FloatLerp::LerpMode _mode)
	{
		switch(_mode)
		{
			case FloatLerp::UNITS_PER_SECOND:
			{
				return "UNITS_PER_SECOND";
			}

			case FloatLerp::PARABOLA:
			{
				return "PARABOLA";
			}

			case FloatLerp::FIXED_TIME:
			{
				return "FIXED_TIME";
			}

			case FloatLerp::DAMPING:
			{
				return "DAMPING";
			}
			
			case FloatLerp::MAGNETIC:
			{
				return "MAGNETIC";
			}
			
		}

		return "";
	}


	// ������� ������������
	public: virtual void Draw(float _delta)
	{
		Engine::Draw(_delta);


		// ������ ����� ������������� �����...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// ����� ������...
		int32 i = 0;



		font->SetColor(graph->FindFunction(selected->GetName())->GetColor());// ���� ������ 
		font->DrawScreenText(Vector3(10, i+=20, -1), "name: %s", selected->GetName().c_str());
		font->DrawScreenText(Vector3(10, i+=20, -1), "playing speed: %f", selected->GetPlayingSpeed());
		font->DrawScreenText(Vector3(10, i+=20, -1), "is playing: %d", selected->IsPlaying());
		font->DrawScreenText(Vector3(10, i+=20, -1), "target: %f", selected->GetTarget());
		font->DrawScreenText(Vector3(10, i+=20, -1), "current: %f", selected->GetCurrent());
		font->DrawScreenText(Vector3(10, i+=20, -1), "y maximum: %f", graph->FindFunction(selected->GetName())->GetYMaximum());
		font->DrawScreenText(Vector3(10, i+=20, -1), "delta: %f", selected->GetDelta());
		font->DrawScreenText(Vector3(10, i+=20, -1), "t: %f", selected->GetT());
		font->DrawScreenText(Vector3(10, i+=20, -1), "time to target (sec): %f", selected->GetTimeToTarget());
		//font->DrawScreenText(Vector3(10, i+=20, -1), "accum: %f", accum[selected->GetLerpMode()]);

		font->DrawScreenText(Vector3(10, i+=20, -1), "duration: %f", selected->GetPlayingDurationInSec());

		
		//font->DrawScreenText(Vector3(10, i+=20, -1), "%f", camera->GetDistanceToViewingPoint());
	

		// ������������ ���������� �������������...
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
	}


	// ��������� ����������� ������� � ��������, � �������� ���������� � ������� ������� � ������ ������
	public: void CheckCollisionWithMarkers(void)
	{
		if(!startMarkerMoving && !rightPressed)// ���� ������ �� ��������� � ������ �� ������������
		{
			if(unitsPerSecond->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(UNITS_PER_SECOND);
				Engine::_PickCursor(unclenchedHandCursor);// �������� ������
			}
			else if(fixedTime->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(FIXED_TIME);
				Engine::_PickCursor(unclenchedHandCursor);// �������� ������
			}
			else if(parabola->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(PARABOLA);
				Engine::_PickCursor(unclenchedHandCursor);// �������� ������
			}
			else if(damping->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(DAMPING);
				Engine::_PickCursor(unclenchedHandCursor);// �������� ������
			}
			else if(magnetic->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(MAGNETIC);
				Engine::_PickCursor(unclenchedHandCursor);// �������� ������
			}
			else
			{
				Unselect();
				Engine::_PickCursor(arrowCursor);// �������� ������
			}
		}
	}
		

	// ���������� ����������������� ������������ � �������� ��� ������ PARABOLA
	float GetParabolaPlayingDurationInSec(void)
	{
		/* ��������� ������� ����������������� ������������...
		 1. ���� ������� ������� �� ����������...
			1.1 ��������� ����� ���� ������������, ��������� 1/60 � �������� ������ �������...
			1.2 ���������� ������� ����� ������������...
		 2. ����� ���������� ������� ����� ������������. */ 

		float current = 0.0f, t = 0.0f, target = parabolaLerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

		float steps;

		while(current != target)
		{  
			steps = (target - current) / (parabolaLerp->GetPlayingSpeed() * delta);
					
			t += 1.0f / steps;
									 
			if(fabs(t) > 1.0f) 
			{ 
				current = target;
			}
			else 
			{
				current = current * (1.0f - t) + target * t;
			}
			duration += delta;
		}
		return duration;
	}



	// ������ ������� ���� ������������ � �������� ��� ������ PARABOLA
	void SetParabolaPlayingDurationInSec(float _duration)
	{
		/* ��������� ������� �������� ��� ������������� ����������������� �������...
		   ��������� ������� ����������� ��� �������, ��� �������� ��������������� 
		   ��������� ����������������� ������������; ��� ��������� ������� ������� 
		   ����� ��������� ����.
		 1.1 ���� ������� ����� ������ ������� ����������������� ������������...
			 1.1.1 ����������� �������� ������������ �� N...
			 1.1.2 ���� ������� ����� ����� ������ ������� ����������������� ������������...
				   1.1.2.1 �������� �������� ������������ �� N...
				   1.1.2.2 ���� K <= 0...
						   1.1.2.2.1 ������������� ����������...
						   ����� N = N / 10...
						   K = K - 1. 

		  target = 1
		  ����������� 0.1 ��� � 16 ������

		  target = 10
		  ����������� 0.1 ��� � 30 ������
		*/ 

		if(_duration >= 0.1f)
		{
			// ��� ������ playingSpeed - ��� ������ �������� �������, � ������ ��� ������ ������������ ������������ 
			int32 steps = 0; // ���������� ��������������� ���� ��������; ����������� ��� ����� ���������� TINY
					
			if(parabolaLerp->GetPlayingSpeed() > 0.0f)
			{
				float scale[4] = { 0.1f, 0.001f, 0.0001f, 0.00001f };

				float step = 1.0f;// ��� ��������� �������� ��������
				float playingSpeed = 0.00001f;// �������� � ������������ ����������������� ������������

				while(true)
				{
					playingSpeed += step;// ��������� ��������
					parabolaLerp->SetPlayingSpeed(playingSpeed);

					if(_duration > GetParabolaPlayingDurationInSec())// ���� ������� ����������������� ������������ ����� ������ �������
					{
						playingSpeed -= step;// �������� ���������� ��������
						parabolaLerp->SetPlayingSpeed(playingSpeed);
									
						// ��������� ���� ������, �� � ������� �����...
						if(steps < 4)// ���� ������� �� ����� 0
						{
							step = scale[steps];// ��������� ������ ����
							++steps;
						}
						else
						{
							break;// �������� ����
						}
					}
				}
			}
			if(parabolaLerp->GetPlayingSpeed() < 0.0f)
			{
				float scale[4] = { -0.1f, -0.001f, -0.0001f, -0.00001f };

				float step = -1.0f;// ��� ��������� �������� ��������
				float playingSpeed = -0.00001f;// �������� � ������������ ����������������� ������������

				while(true)
				{
					playingSpeed += step;// ��������� ��������
					parabolaLerp->SetPlayingSpeed(playingSpeed);

					if(_duration > GetParabolaPlayingDurationInSec())// ���� ������� ����������������� ������������ ����� ������ �������
					{
						playingSpeed -= step;// �������� ���������� ��������
						parabolaLerp->SetPlayingSpeed(playingSpeed);
									
						// ��������� ���� ������, �� � ������� �����...
						if(steps < 4)// ���� ������� �� ����� 0
						{
							step = scale[steps];// ��������� ������ ����
							++steps;
						}
						else
						{
							break;// �������� ����
						}
					}
				}
			}
		}
	}

	
	// ���������� ����������������� ������������ � �������� ��� ������ MAGNETIC
	float GetMagneticPlayingDurationInSec(void)
	{
		/* ��������� ������� ����������������� ������������...
		 1. ���� ������� ������� �� ����������...
			1.1 ��������� ����� ���� ������������, ��������� 1/60 � �������� ������ �������...
			1.2 ���������� ������� ����� ������������...
		 2. ����� ���������� ������� ����� ������������. */ 

		float current = 0.0f, t = 0.0f, target = magneticLerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

		float steps;

		while(current != target)
		{
			steps = (target - current) / (magneticLerp->GetPlayingSpeed() * delta);
			
			t += 1.0f / steps;
							
			if(fabs(t) > 1.0f)
			{  
				current = target;
			}
			else 
			{ 
				current = target * t;
			}
			duration += delta;
		}
		return duration;
	}

	// ������ ������� ���� ������������ � �������� ��� ������ MAGNETIC
	void SetMagneticPlayingDurationInSec(float _duration)
	{
		/* ��������� ������� �������� ��� ������������� ����������������� �������...
		   ��������� ������� ����������� ��� �������, ��� �������� ��������������� 
		   ��������� ����������������� ������������; ��� ��������� ������� ������� 
		   ����� ��������� ����.
		 N = 1
		 K = 7
		 1.1 ���� ������� ����� ������ ������� ����������������� ������������...
			 1.1.1 ����������� �������� ������������ �� N...
			 1.1.2 ���� ������� ����� ����� ������ ������� ����������������� ������������...
				   1.1.2.1 �������� �������� ������������ �� N...
				   1.1.2.2 ���� K <= 0...
						   1.1.2.2.1 ������������� ����������...
						   ����� N = N / 10...
						   K = K - 1. 
		  target = 1
		  ����������� 0.1 ��� � 77 ������

		  target = 10
		  ����������� 0.1 ��� � 231 ������
		*/ 

		if(_duration >= 0.3f)
		{
			// ��� ������ playingSpeed - ��� ������ �������� �������, � ������ ��� ������ ������������ ������������ 
			int32 steps = 0; // ���������� ��������������� ���� ��������; ����������� ��� ����� ���������� TINY
					
			if(magneticLerp->GetPlayingSpeed() > 0.0f)
			{
				float scale[4] = { 0.1f, 0.001f, 0.0001f, 0.00001f };

				float step = 1.0f;// ��� ��������� �������� ��������
				float playingSpeed = 0.00001f;// �������� � ������������ ����������������� ������������

				while(true)
				{
					playingSpeed += step;// ��������� ��������
					magneticLerp->SetPlayingSpeed(playingSpeed);

					if(_duration > GetMagneticPlayingDurationInSec())// ���� ������� ����������������� ������������ ����� ������ �������
					{
						playingSpeed -= step;// �������� ���������� ��������
						magneticLerp->SetPlayingSpeed(playingSpeed);
									
						// ��������� ���� ������, �� � ������� �����...
						if(steps < 4)// ���� ������� �� ����� 0
						{
							step = scale[steps];// ��������� ������ ����
							++steps;
						}
						else
						{
							break;// �������� ����
						}
					}
				}
			}
			if(magneticLerp->GetPlayingSpeed() < 0.0f)
			{
				float scale[4] = { -0.1f, -0.001f, -0.0001f, -0.00001f };

				float step = -1.0f;// ��� ��������� �������� ��������
				float playingSpeed = -0.00001f;// �������� � ������������ ����������������� ������������

				while(true)
				{
					playingSpeed += step;// ��������� ��������
					magneticLerp->SetPlayingSpeed(playingSpeed);

					if(_duration > GetMagneticPlayingDurationInSec())// ���� ������� ����������������� ������������ ����� ������ �������
					{
						playingSpeed -= step;// �������� ���������� ��������
						magneticLerp->SetPlayingSpeed(playingSpeed);
									
						// ��������� ���� ������, �� � ������� �����...
						if(steps < 4)// ���� ������� �� ����� 0
						{
							step = scale[steps];// ��������� ������ ����
							++steps;
						}
						else
						{
							break;// �������� ����
						}
					}
				}
			}
		}
	}

	// ���������� ����������������� ������������ � �������� ��� ������ DAMPING
	float GetDampingPlayingDurationInSec(void)
	{
		/* ��������� ������� ����������������� ������������...
		 1. ���� ������� ������� �� ����������...
			1.1 ��������� ����� ���� ������������, ��������� 1/60 � �������� ������ �������...
			1.2 ���������� ������� ����� ������������...
		 2. ����� ���������� ������� ����� ������������. */ 

		float current = 0.0f, t = 0.0f, target = dampingLerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

		float step = 1.0f / fabs(dampingLerp->GetPlayingSpeed()) * delta;

		while(current != target)
		{
			float previous = current;
		 
			current = current * (1.0f - step) + target * step;
						
			if(fabs(current) >= fabs(target))
			{ 
				current = target; 
			}
			else if(fabs(current - previous) <= 0.000001f)
			{
				current = target;
			}
			duration += delta;
		}
		return duration;
	} 
		

	// ������ ������� ���� ������������ � �������� ��� ������ DAMPING
	void SetDampingPlayingDurationInSec(float _duration)
	{
		/* ��������� ������� �������� ��� ������������� ����������������� �������...
		   ��������� ������� ����������� ��� �������, ��� �������� ��������������� 
		   ��������� ����������������� ������������; ��� ��������� ������� ������� 
		   ����� ��������� ����.

		 1.1 ���� ������� ����� ������ ������� ����������������� ������������...
			 1.1.1 ����������� �������� ������������ �� N...
			 1.1.2 ���� ������� ����� ����� ������ ������� ����������������� ������������...
				   1.1.2.1 �������� �������� ������������ �� N...
				   1.1.2.2 ���� K <= 0...
						   1.1.2.2.1 ������������� ����������...
						   ����� N = N / 10...
						   K = K - 1. 
		  target = 1
		  ����������� 0.1 ��� � - ������

		  target = 10
		  ����������� 0.1 ��� � - ������
		*/ 

		if(_duration >= 0.1f)
		{
			/* ��� ������ playingSpeed - ��� ������ �������� �������, � ������ ��� ������ ������������ ������������ */
			int32 steps = 6; // ���������� ��������������� ���� ��������; ����������� ��� ����� ���������� TINY
					
			if(dampingLerp->GetPlayingSpeed() > 0.0f)
			{
				float step = 1.0f;// ��� ��������� �������� ��������
				float playingSpeed = step; 
				dampingLerp->SetPlayingSpeed(playingSpeed);


				if(_duration > GetDampingPlayingDurationInSec())
				{
					while(_duration > GetDampingPlayingDurationInSec())// ���� ������� ����������������� ������� ������ ������� 
					{
						bool scale = false;

						if((playingSpeed + step) > 0.0f)// ���� ����� �������� ����� ������ 0
						{
							playingSpeed += step;// ��������� ��������
							dampingLerp->SetPlayingSpeed(playingSpeed);
						}
						else
						{
							scale = true;// ��������� ������ ����
						}

						if(_duration < GetDampingPlayingDurationInSec())// ���� ������� ����������������� ������� ����� ������ �������
						{
							playingSpeed -= step;// �������� ���������� ��������
							dampingLerp->SetPlayingSpeed(playingSpeed);
							scale = true;// ��������� ������ ����
						}

						if(scale)
						{
							// ��������� ���� ������, �� � ������� �����...
							if(steps > 0)// ���� ������� �� ����� 0
							{
								--steps;// ��������� ������� ���������������
								step /= 10.0f;// ��������� ������ ����
							}
							else
							{
								break;// �������� ����
							}
						}
					}
				}
				else if(_duration < GetDampingPlayingDurationInSec())
				{
					while(_duration < GetDampingPlayingDurationInSec())// ���� ������� ����������������� ������� ������ ������� 
					{ 
						bool scale = false;

						if((playingSpeed - step) > 0.0f)// ���� ����� �������� ����� ������ 0
						{
							playingSpeed -= step;// ��������� ��������
							dampingLerp->SetPlayingSpeed(playingSpeed);
						}
						else
						{
							scale = true;// ��������� ������ ����
						}

						if(_duration > GetDampingPlayingDurationInSec())// ���� ������� ����������������� ������� ����� ������ �������
						{
							playingSpeed += step;// �������� ���������� ��������
							dampingLerp->SetPlayingSpeed(playingSpeed);
							scale = true;// ��������� ������ ����
						}

						if(scale)
						{
							// ��������� ���� ������, �� � ������� �����...
							if(steps > 0)// ���� ������� �� ����� 0
							{
								--steps;// ��������� ������� ���������������
								step /= 10.0f;// ��������� ������ ����
							}
							else
							{
								break;// �������� ����
							}
						}
					}
				}
			} 
			else if(dampingLerp->GetPlayingSpeed() < 0.0f) 
			{ 
				float step = -1.0f;// ��� ��������� �������� ��������
				float playingSpeed = step;
				dampingLerp->SetPlayingSpeed(playingSpeed);

				if(_duration > GetDampingPlayingDurationInSec())
				{
					while(_duration > GetDampingPlayingDurationInSec())// ���� ������� ����������������� ������� ������ ������� 
					{
						bool scale = false;

						if((playingSpeed + step) < 0.0f)// ���� ����� �������� ����� ������ 0
						{
							playingSpeed += step;// ��������� ��������
							dampingLerp->SetPlayingSpeed(playingSpeed);
						}
						else
						{
							scale = true;// ��������� ������ ����
						}

						if(_duration < GetDampingPlayingDurationInSec())// ���� ������� ����������������� ������� ����� ������ �������
						{
							playingSpeed -= step;// �������� ���������� ��������
							dampingLerp->SetPlayingSpeed(playingSpeed);
							scale = true;// ��������� ������ ����
						}

						if(scale)
						{
							// ��������� ���� ������, �� � ������� �����...
							if(steps > 0)// ���� ������� �� ����� 0
							{
								--steps;// ��������� ������� ���������������
								step /= 10.0f;// ��������� ������ ����
							}
							else
							{
								break;// �������� ����
							}
						}
					}
				} 
				else if(_duration < GetDampingPlayingDurationInSec())
				{
					while(_duration < GetDampingPlayingDurationInSec())// ���� ������� ����������������� ������� ������ ������� 
					{ 
						bool scale = false;

						if((playingSpeed - step) < 0.0f)// ���� ����� �������� ����� ������ 0
						{
							playingSpeed -= step;// ��������� ��������
							dampingLerp->SetPlayingSpeed(playingSpeed);
						}
						else
						{
							scale = true;// ��������� ������ ����
						}

						if(_duration > GetDampingPlayingDurationInSec())// ���� ������� ����������������� ������� ����� ������ �������
						{
							playingSpeed += step;// �������� ���������� ��������
							dampingLerp->SetPlayingSpeed(playingSpeed);
							scale = true;// ��������� ������ ����
						}

						if(scale)
						{
							// ��������� ���� ������, �� � ������� �����...
							if(steps > 0)// ���� ������� �� ����� 0
							{
								--steps;// ��������� ������� ���������������
								step /= 10.0f;// ��������� ������ ����
							}
							else
							{
								break;// �������� ����
							}
						}
					}
				}
			}
		}
	}



	// ������ ������� ����
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);

		if(_event->type == Mouse::KEY_DOWN)// ���� ������ ������
		{
			if(_event->key == Mouse::LEFT_BUTTON)// ���� ����� ����
			{
				Vector3 currentMarkerMoving;

				if(graph->GetPlane().IsUnderCursor(currentMarkerMoving) != -1)// ���� ���� ����������� ������� � ���������� �������
				{
					// ������ ����������� �������...
					if(selectedMarker != NO_SELECT && !rightPressed)// ���� ������ �� ���������, � ���� ������ ��� ��������
					{
						startMarkerMoving = true;
						Engine::_PickCursor(clenchedHandCursor);// �������� ������
						
						startMarkerPosition = currentMarkerMoving;// ��������� ��������� �������
					}
				}
			}
		}
		else if(_event->type == Mouse::KEY_PRESSED)
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// ������ �������� ������
			{
				// ��������� �������� ������ ������ ��� ������� ������ �����

				if(!rightPressed)// ��������� ������ ���� ���
				{
					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// ���������� ���� � ����� ���������� ������� ���� 
					camera->SetRotationEnable(true);// ��������� �������� ������
					rightPressed = true;
					Engine::_PickCursor(NIL);
					camera->SetZoomEnable(false);// ��������� ������
					Unselect();
					//Mouse::_SetShowSystemCursor(false);// ������ ������
				}
			}
		}
		else if(_event->type == Mouse::KEY_UP)// ����� ������ ��������
		{
			if(_event->key == Mouse::LEFT_BUTTON)// ���� ����� ����
			{
				// ����� ����������� �������...
				if(startMarkerMoving) 
				{ 
					startMarkerMoving = false; // ��������� �����������
					Engine::_PickCursor(unclenchedHandCursor);// �������� ������

					switch(selectedMarker)
					{
						case UNITS_PER_SECOND:
						{
							// ������������� ������������ �������
							unitsPerSecond->ClearHistory();// �������� ������� �������
							// ��������� ������� �������
							unitsPerSecondLerp->Stop();
							unitsPerSecondLerp->Play();
						}
							
						case FIXED_TIME:
						{
							// ������������� ������������ �������
							fixedTime->ClearHistory();// �������� ������� �������
							// ��������� ������� �������
							fixedTimeLerp->Stop();
							fixedTimeLerp->Play();
						}

						case PARABOLA:
						{
							// ������������� ������������ �������
							parabola->ClearHistory();// �������� ������� �������
							// ��������� ������� �������
							parabolaLerp->Stop();
							parabolaLerp->Play();
						}

						case MAGNETIC:
						{
							// ������������� ������������ �������
							magnetic->ClearHistory();// �������� ������� �������
							// ��������� ������� �������
							magneticLerp->Stop();
							magneticLerp->Play();
						}

						case DAMPING:
						{
							// ������������� ������������ �������
							damping->ClearHistory();// �������� ������� �������
							// ��������� ������� �������
							dampingLerp->Stop();
							dampingLerp->Play();
						}
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// ����� �������� ������
			{
				if(rightPressed)// ��������� ������ ���� ���
				{
					camera->SetRotationEnable(false); // ��������� �������� ������
					rightPressed = false;
					Engine::_PickCursor(arrowCursor);
					camera->SetZoomEnable(true);// ��������� ������
					//Mouse::_SetShowSystemCursor(true);// �������� ������
				}
			}
		}	
		else if(_event->type == Mouse::WHEEL)// �������� �������� ����
		{
			if(!rightPressed)// ���� ������ �� ���������
			{
				Unselect();
				Engine::_PickCursor(arrowCursor);// �������� ������
			}
		}
		else if(_event->type == Mouse::MOVE)// �������� �������
		{
			if(startMarkerMoving) // ���� ������ ������������
			{
				Vector3 currentCursorPosition;

				if(graph->GetPlane().IsUnderCursor(currentCursorPosition) != -1)// ���� ���� ����������� ������� � ���������� �������
				{
					//Vector3 v = Vector3::_FromTo(startMarkerPosition, currentMarkerPosition);// ������ �� ��������� ����� � �������

					switch(selectedMarker)
					{
						case UNITS_PER_SECOND:
						{
							// ��� ����� ����� ����� �� ������� �� ��������� �����������, ������� ������������� ����� ������� �� ��������� �� ��� X
							// ������� ������������� ���������� ��������� � ����������: t = a * b / c - d, ����� �������� �������: a = (t + d) / b * c, ��� a - ���������� ����������, t  - ���������

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// �� ���������� ����� ����������� ���������������, ����� �� ������ ������� ����������� 1 �������
							Vector2 halfSize = graph->GetSize() / 2.0f;// �������� �� ������� �������
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// ������ ��� ����� ����� ����� �������� ��������, ����� ���� ���� ���������� �� targetTime ������; ��� ����� �� ������������� ���������� Target / Time = MoveSpeed
							unitsPerSecond->ClearHistory();// �������� ������� �������
							unitsPerSecondLerp->SetPlayingDurationInSec(targetTime);
							// ��������� ������� �������
							unitsPerSecondLerp->Stop();
							unitsPerSecondLerp->Play();


							// ��������� ������� �� ���� ������ �������; ����� ������� �� �������� ������ ������ ������� ������� ��������� ��� ������� ����������
							while(unitsPerSecondLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								unitsPerSecondLerp->Update(fixedDelta);
								graph->AddVertex(unitsPerSecondLerp->GetName(), Vector2(fixedDelta, unitsPerSecondLerp->GetCurrent()));
							}
							// �������� ������� ������� �� ��� �������....
							graph->GetMarkList()[0]->key = Graph::Mark(selected->GetPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}
						
						case FIXED_TIME:
						{
							// ��� ����� ����� ����� �� ������� �� ��������� �����������, ������� ������������� ����� ������� �� ��������� �� ��� X
							// ������� ������������� ���������� ��������� � ����������: t = a * b / c - d, ����� �������� �������: a = (t + d) / b * c, ��� a - ���������� ����������, t  - ���������

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// �� ���������� ����� ����������� ���������������, ����� �� ������ ������� ����������� 1 �������
							Vector2 halfSize = graph->GetSize() / 2.0f;// �������� �� ������� �������
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// ������ ��� ����� ����� ����� �������� ��������, ����� ���� ���� ���������� �� targetTime ������
							fixedTime->ClearHistory();// �������� ������� �������
							fixedTimeLerp->SetPlayingDurationInSec(targetTime);// ������������ SetMoveSpeed(targetTime)
							// ��������� ������� �������
							fixedTimeLerp->Stop();
							fixedTimeLerp->Play();


							// ��������� ������� �� ���� ������ �������; ����� ������� �� �������� ������ ������ ������� ������� ��������� ��� ������� ����������
							while(fixedTimeLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								fixedTimeLerp->Update(fixedDelta);
								graph->AddVertex(fixedTimeLerp->GetName(), Vector2(fixedDelta, fixedTimeLerp->GetCurrent()));
							}
							
							// �������� ������� ������� �� ��� �������....
							graph->GetMarkList()[0]->key = Graph::Mark(selected->GetPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}

						case PARABOLA:
						{
							// ��� ����� ����� ����� �� ������� �� ��������� �����������, ������� ������������� ����� ������� �� ��������� �� ��� X
							// ������� ������������� ���������� ��������� � ����������: t = a * b / c - d, ����� �������� �������: a = (t + d) / b * c, ��� a - ���������� ����������, t  - ���������

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// �� ���������� ����� ����������� ���������������, ����� �� ������ ������� ����������� 1 �������
							Vector2 halfSize = graph->GetSize() / 2.0f;// �������� �� ������� �������
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// ������ ��� ����� ����� ����� �������� ��������, ����� ���� ���� ���������� �� targetTime ������
							parabola->ClearHistory();// �������� ������� �������
							SetParabolaPlayingDurationInSec(targetTime);
							// ��������� ������� �������
							parabolaLerp->Stop();
							parabolaLerp->Play();


							// ��������� ������� �� ���� ������ �������; ����� ������� �� �������� ������ ������ ������� ������� ��������� ��� ������� ����������
							while(parabolaLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								parabolaLerp->Update(fixedDelta);
								graph->AddVertex(parabolaLerp->GetName(), Vector2(fixedDelta, parabolaLerp->GetCurrent()));
							}
							// �������� ������� ������� �� ��� �������....
							graph->GetMarkList()[0]->key = Graph::Mark(GetParabolaPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}
						
						case DAMPING:
						{
							// ��� ����� ����� ����� �� ������� �� ��������� �����������, ������� ������������� ����� ������� �� ��������� �� ��� X
							// ������� ������������� ���������� ��������� � ����������: t = a * b / c - d, ����� �������� �������: a = (t + d) / b * c, ��� a - ���������� ����������, t  - ���������

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// �� ���������� ����� ����������� ���������������, ����� �� ������ ������� ����������� 1 �������
							Vector2 halfSize = graph->GetSize() / 2.0f;// �������� �� ������� �������
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// ������ ��� ����� ����� ����� �������� ��������, ����� ���� ���� ���������� �� targetTime ������
							damping->ClearHistory();// �������� ������� �������
							SetDampingPlayingDurationInSec(targetTime);
							// ��������� ������� �������
							dampingLerp->Stop();
							dampingLerp->Play();
							

							// ��������� ������� �� ���� ������ �������; ����� ������� �� �������� ������ ������ ������� ������� ��������� ��� ������� ����������
							while(dampingLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								dampingLerp->Update(fixedDelta);
								graph->AddVertex(dampingLerp->GetName(), Vector2(fixedDelta, dampingLerp->GetCurrent()));
							}
							// �������� ������� ������� �� ��� �������....
							graph->GetMarkList()[0]->key = Graph::Mark(GetDampingPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}
						
						case MAGNETIC:
						{
							// ��� ����� ����� ����� �� ������� �� ��������� �����������, ������� ������������� ����� ������� �� ��������� �� ��� X
							// ������� ������������� ���������� ��������� � ����������: t = a * b / c - d, ����� �������� �������: a = (t + d) / b * c, ��� a - ���������� ����������, t  - ���������

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// �� ���������� ����� ����������� ���������������, ����� �� ������ ������� ����������� 1 �������
							Vector2 halfSize = graph->GetSize() / 2.0f;// �������� �� ������� �������
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// ������ ��� ����� ����� ����� �������� ��������, ����� ���� ���� ���������� �� targetTime ������
							magnetic->ClearHistory();// �������� ������� �������
							SetMagneticPlayingDurationInSec(targetTime);
							// ��������� ������� �������
							magneticLerp->Stop();
							magneticLerp->Play();
							

							// ��������� ������� �� ���� ������ �������; ����� ������� �� �������� ������ ������ ������� ������� ��������� ��� ������� ����������
							while(magneticLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								magneticLerp->Update(fixedDelta);
								graph->AddVertex(magneticLerp->GetName(), Vector2(fixedDelta, magneticLerp->GetCurrent()));
							}
							// �������� ������� ������� �� ��� �������....
							graph->GetMarkList()[0]->key = Graph::Mark(GetMagneticPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}
					}

												
					
					//startMarkerPosition = currentMarkerPosition;// �������� ��������� �����
				}
			}
		}
		return _event;
	}


	// �������� ������ ������� _funcId
	public: void Select(int32 _funcId)
	{
		if(selectedMarker != _funcId)// ���� ����� �������� ����� ������
		{
			Unselect();// ����� ��������� � ����������� �������

			selectedMarker = _funcId;
			Engine::_PickCursor(unclenchedHandCursor);// �������� ������
		}
	}


	// ������� ��������� � ����������� �������
    public: void Unselect(void)
	{
		selectedMarker = NO_SELECT;
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
			else if(_event->key == Keyboard::C)
			{
				graph->ClearHistory();
			} 
			
			else if(_event->key == Keyboard::S)
			{
				graph->ClearHistory();
				
				unitsPerSecondLerp->Stop();
				unitsPerSecondLerp->Play();

				fixedTimeLerp->Stop();
				fixedTimeLerp->Play();

				parabolaLerp->Stop();
				parabolaLerp->Play();
				
				dampingLerp->Stop();
				dampingLerp->Play();

				magneticLerp->Stop();
				magneticLerp->Play();
			} 
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				index--;

				if(index < 0)
				{
					SelectFunction(4);
				}
				else { SelectFunction(index); }
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				index++;  

				if(index > 4) 
				{
					SelectFunction(0);
				}
				else { SelectFunction(index); }
			}
		}

		return Engine::KeyboardEventFilter(_event);
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
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // ��������� ������ small.ico �� ��������
	def.rendererDef.realTimeWorkerDef.windowDef.title = "FloatLerp.example_01";
	//def.rendererDef.zBufferEnabled = false;
	Application* app = new Application();// ������� ����
	
	if(app->Create(def))
	{
		app->Loop();// ��������� ���� �������
	}
	delete app;
	return 0;
}
