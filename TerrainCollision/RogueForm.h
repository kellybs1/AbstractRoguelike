#pragma once

#include "GameManager.h"

#define BAR_WIDTH 40
#define BAR_HEIGHT 200
#define BORDER_GAP 50

namespace TerrainCollision {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for RogueForm
	/// </summary>
	public ref class RogueForm : public System::Windows::Forms::Form
	{
	public:
		RogueForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RogueForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		Graphics^ mainCanvas;
		Random^ rand;
		Graphics^ bufferGraphics;
		Bitmap^ bufferImage;
		Brush^ bgFill;
		GameManager^ mainManager;
		Brush^ manaFill;
		Brush^ healthFill;
		Brush^ emptyBarFill;


	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  labelHealth;
	private: System::Windows::Forms::Label^  labelMana;
	private: System::Windows::Forms::Label^  labelScore;
	private: System::Windows::Forms::Label^  labelLevel;

	private: System::ComponentModel::IContainer^  components;





#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(RogueForm::typeid));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->labelHealth = (gcnew System::Windows::Forms::Label());
			this->labelMana = (gcnew System::Windows::Forms::Label());
			this->labelScore = (gcnew System::Windows::Forms::Label());
			this->labelLevel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 30;
			this->timer1->Tick += gcnew System::EventHandler(this, &RogueForm::timer1_Tick);
			// 
			// labelHealth
			// 
			this->labelHealth->AutoSize = true;
			this->labelHealth->Location = System::Drawing::Point(27, 367);
			this->labelHealth->Name = L"labelHealth";
			this->labelHealth->Size = System::Drawing::Size(35, 13);
			this->labelHealth->TabIndex = 0;
			this->labelHealth->Text = L"label1";
			this->labelHealth->Visible = false;
			// 
			// labelMana
			// 
			this->labelMana->AutoSize = true;
			this->labelMana->Location = System::Drawing::Point(27, 414);
			this->labelMana->Name = L"labelMana";
			this->labelMana->Size = System::Drawing::Size(35, 13);
			this->labelMana->TabIndex = 1;
			this->labelMana->Text = L"label1";
			this->labelMana->Visible = false;
			// 
			// labelScore
			// 
			this->labelScore->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelScore->Font = (gcnew System::Drawing::Font(L"Courier New", 13.71428F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelScore->Location = System::Drawing::Point(27, 460);
			this->labelScore->Name = L"labelScore";
			this->labelScore->Size = System::Drawing::Size(100, 30);
			this->labelScore->TabIndex = 2;
			this->labelScore->Text = L"score";
			this->labelScore->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// labelLevel
			// 
			this->labelLevel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->labelLevel->Font = (gcnew System::Drawing::Font(L"Courier New", 13.71428F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelLevel->Location = System::Drawing::Point(444, 21);
			this->labelLevel->Name = L"labelLevel";
			this->labelLevel->Size = System::Drawing::Size(140, 30);
			this->labelLevel->TabIndex = 3;
			this->labelLevel->Text = L"level goes here";
			this->labelLevel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// RogueForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(1022, 694);
			this->Controls->Add(this->labelLevel);
			this->Controls->Add(this->labelScore);
			this->Controls->Add(this->labelMana);
			this->Controls->Add(this->labelHealth);
			this->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"RogueForm";
			this->Text = L"Abstract Roguelike";
			this->Load += gcnew System::EventHandler(this, &RogueForm::RogueForm_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &RogueForm::RogueForm_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &RogueForm::RogueForm_KeyUp);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void RogueForm_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		Bitmap^ playerBitmap = gcnew Bitmap("images/characters/playerSingleFrame.png");
		//variable constants
		Width = 1296; // 1296
		Height = 680; // 680 // 800 for small map view
		labelScore->Width = 220;
		labelScore->Left = (ClientRectangle.Width / 2) - (labelScore->Width / 2) + (playerBitmap->Width / 2);
		labelScore->Top = ClientRectangle.Height - labelScore->Height - BORDER_GAP;
		labelLevel->Width = 140;
		labelLevel->Left = (ClientRectangle.Width / 2) - (labelLevel->Width / 2) + (playerBitmap->Width / 2);
		labelLevel->Top = ClientRectangle.Top + BORDER_GAP / 2;
		Top = 0;
		Left = -6;
		mainCanvas = CreateGraphics();
		bgFill = gcnew SolidBrush(Color::White);
		healthFill = gcnew SolidBrush(Color::Red);
		manaFill = gcnew SolidBrush(Color::Blue);
		emptyBarFill = gcnew SolidBrush(Color::FromArgb(59, 63, 68));
		bufferImage = gcnew Bitmap(ClientRectangle.Width, ClientRectangle.Height);
		bufferGraphics = Graphics::FromImage(bufferImage);
		rand = gcnew Random();
		mainManager = gcnew GameManager(bufferGraphics, rand, ClientRectangle);
		timer1->Enabled = true;
	}

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{	
		mainManager->GameCycle();
		updateLabels();
		drawStatusBars();
		mainCanvas->DrawImage(bufferImage, 0, 0); 
		GC::Collect(0);
	}



	//draws player's health and mana to rectangles on form
	private: System::Void drawStatusBars()
	{
		//get player values
		int playerHealth = mainManager->PlayerHealth;
		int playerMana = mainManager->PlayerMana;
		//calculate size of each step in status bar
		int notchHealth = BAR_HEIGHT / SPRITE_HEALTH_MAX; //these are best left at 200 and 100
		int notchMana = BAR_HEIGHT / SPRITE_MANA_MAX;
		//place the x positions
		int healthBarX = ClientRectangle.Left + BORDER_GAP;
		int manaBarX = ClientRectangle.Right - BAR_WIDTH - BORDER_GAP;
		//set the main y poisition
		int BothBarStartY = ClientRectangle.Bottom - BAR_HEIGHT - BORDER_GAP;
		//set height based on number of notches
		int healthBarHeight = playerHealth * notchHealth;
		int manaBarHeight = playerMana * notchMana;
		//set the starting y position based on the status value
		int healthBarY = BothBarStartY + ((SPRITE_HEALTH_MAX - playerHealth) * notchHealth);
		int manaBarY = BothBarStartY + ((SPRITE_MANA_MAX - playerMana) * notchMana);
		//make background rectangles
		Rectangle healthBGRect = Rectangle(healthBarX, BothBarStartY, BAR_WIDTH, BAR_HEIGHT);
		Rectangle manaBGRect = Rectangle(manaBarX, BothBarStartY, BAR_WIDTH, BAR_HEIGHT);
		//make foreground rectangles
		Rectangle healthRect = Rectangle(healthBarX,healthBarY, BAR_WIDTH, healthBarHeight);
		Rectangle manaRect = Rectangle(manaBarX, manaBarY, BAR_WIDTH, manaBarHeight);
		//draw
		bufferGraphics->FillRectangle(emptyBarFill, healthBGRect);
		bufferGraphics->FillRectangle(emptyBarFill, manaBGRect);
		bufferGraphics->FillRectangle(healthFill, healthRect);
		bufferGraphics->FillRectangle(manaFill, manaRect);

	}

	//update the health, mana, score labels on the form
	private: System::Void updateLabels()
	{
			 int playerHealth = mainManager->PlayerHealth;
			 int playerMana = mainManager->PlayerMana;
			 int playerScore = mainManager->PlayerScore;
			 int level = mainManager->Level + 1;
			 String^ healthString = Convert::ToString(playerHealth);
			 String^ manaString = Convert::ToString(playerMana);
			 String^ scoreString = "Score: " + Convert::ToString(playerScore);
			 String^ levelString = "Level " + Convert::ToString(level);
			 labelHealth->Text = healthString;
			 labelMana->Text = manaString;
			 labelScore->Text = scoreString;
			 labelLevel->Text = levelString;
	}

	private: System::Void RogueForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		switch (e->KeyData)
		{
			case Keys::A:
				mainManager->LeftDown = false;
				break;

			case Keys::D:
				mainManager->RightDown = false;
				break;

			case Keys::W:
				mainManager->UpDown = false;
				break;

			case Keys::S:
				mainManager->DownDown = false;
				break;
		}

		switch (e->KeyData)
		{
		case Keys::Left:
			//mainManager->makeBullet(WEST);
			mainManager->BulletManaManage(WEST);
			break;

		case Keys::Right:
			//mainManager->makeBullet(EAST);
			mainManager->BulletManaManage(EAST);
			break;

		case Keys::Up:
			//mainManager->makeBullet(NORTH);
			mainManager->BulletManaManage(NORTH);
			break;

		case Keys::Down:
			//mainManager->makeBullet(SOUTH);
			mainManager->BulletManaManage(SOUTH);
			break;

		//testing generate new dungeon
		case Keys::P:
			mainManager->NextDungeon();
			break;

		//new game
		case Keys::N:
			Application::Restart();
			break;

		}

	}


	private: System::Void RogueForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
	{
		switch (e->KeyData)
		{
		case Keys::A:
			mainManager->LeftDown = true;
			break;

		case Keys::D:
			mainManager->RightDown = true;
			break;

		case Keys::W:
			mainManager->UpDown = true;
			break;

		case Keys::S:
			mainManager->DownDown = true;
			break;

		}
	}


};
}
