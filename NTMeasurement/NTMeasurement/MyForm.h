#pragma once
#pragma comment(linker, "/subsystem:console /entry:WinMainCRTStartup")

#include "stdio.h"
#include "ImageProcessing.h"
#include "chanvese.h"
#include "stack"

namespace NTMeasurement {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: 
		System::Windows::Forms::OpenFileDialog^  openFileDialog1;
		Bitmap ^openImage, ^ROIImage, ^PreProImage, ^InitialSegmentation, ^mask;
		Rectangle RcDraw;
		bool _draw;
		int *_mask;
		ImageProcessing *ImageProcessingPtr;

	protected: 
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  Load_MSP_US_Image;
	private: System::Windows::Forms::ToolStripMenuItem^  nTMeasurementToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  segmentationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  initialSegmentationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  seedToolStripMenuItem;



	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器
		/// 修改這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Load_MSP_US_Image = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->nTMeasurementToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->initialSegmentationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->seedToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->segmentationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// pictureBox1
			// 
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox1->Location = System::Drawing::Point(71, 97);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(305, 265);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::pictureBox1_Paint);
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseDown);
			this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseUp);
			// 
			// pictureBox2
			// 
			this->pictureBox2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox2->Location = System::Drawing::Point(404, 97);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(305, 265);
			this->pictureBox2->TabIndex = 1;
			this->pictureBox2->TabStop = false;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->nTMeasurementToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(789, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->Load_MSP_US_Image});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// Load_MSP_US_Image
			// 
			this->Load_MSP_US_Image->Name = L"Load_MSP_US_Image";
			this->Load_MSP_US_Image->Size = System::Drawing::Size(190, 22);
			this->Load_MSP_US_Image->Text = L"Load MSP US Image";
			this->Load_MSP_US_Image->Click += gcnew System::EventHandler(this, &MyForm::Load_MSP_US_Image_Click);
			// 
			// nTMeasurementToolStripMenuItem
			// 
			this->nTMeasurementToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->initialSegmentationToolStripMenuItem, 
				this->seedToolStripMenuItem, this->segmentationToolStripMenuItem});
			this->nTMeasurementToolStripMenuItem->Name = L"nTMeasurementToolStripMenuItem";
			this->nTMeasurementToolStripMenuItem->Size = System::Drawing::Size(117, 20);
			this->nTMeasurementToolStripMenuItem->Text = L"NT Measurement";
			// 
			// initialSegmentationToolStripMenuItem
			// 
			this->initialSegmentationToolStripMenuItem->Name = L"initialSegmentationToolStripMenuItem";
			this->initialSegmentationToolStripMenuItem->Size = System::Drawing::Size(187, 22);
			this->initialSegmentationToolStripMenuItem->Text = L"Initial Segmentation";
			this->initialSegmentationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::initialSegmentationToolStripMenuItem_Click);
			// 
			// seedToolStripMenuItem
			// 
			this->seedToolStripMenuItem->Name = L"seedToolStripMenuItem";
			this->seedToolStripMenuItem->Size = System::Drawing::Size(187, 22);
			this->seedToolStripMenuItem->Text = L"Seed";
			this->seedToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::seedToolStripMenuItem_Click);
			// 
			// segmentationToolStripMenuItem
			// 
			this->segmentationToolStripMenuItem->Name = L"segmentationToolStripMenuItem";
			this->segmentationToolStripMenuItem->Size = System::Drawing::Size(187, 22);
			this->segmentationToolStripMenuItem->Text = L"Final Segmentation";
			this->segmentationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::segmentationToolStripMenuItem_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(789, 460);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"NTMeasurement";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void Load_MSP_US_Image_Click(System::Object^  sender, System::EventArgs^  e) {
				
			if( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{ 
				openImage = gcnew Bitmap(openFileDialog1->FileName);
			
				int width, height;
				width = openImage->Width;
				height = openImage->Height;

				RcDraw.Empty;
				_draw = false;

				pictureBox1->Image = openImage;
			}
		}

private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 
			 RcDraw.X = e->X;
			 RcDraw.Y = e->Y;
		}

private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {		
			
		if(e->X < RcDraw.X)
		{
			RcDraw.Width = RcDraw.X - e->X;
			RcDraw.X = e->X;
		}
		else
		{
			RcDraw.Width = e->X - RcDraw.X;
		}

		if(e->Y < RcDraw.Y)
		{
			RcDraw.Height = RcDraw.Y - e->Y;
			RcDraw.Y = e->Y;
		}
		else
		{
			RcDraw.Height = e->Y - RcDraw.Y;
		}

		// Force a repaint of the region occupied by the rectangle...
		_draw = true;
		this->Invalidate(RcDraw);       
		this->Refresh();  
}

private: System::Void pictureBox1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			
			if(e->X == RcDraw.X || e->Y == RcDraw.Y)
				 return;
			
			if(e->X < RcDraw.X)
			{
				RcDraw.Width = RcDraw.X - e->X;
				RcDraw.X = e->X;
			}
			else
			{
				RcDraw.Width = e->X - RcDraw.X;
			}

			if(e->Y < RcDraw.Y)
			{
				RcDraw.Height = RcDraw.Y - e->Y;
				RcDraw.Y = e->Y;
			}
			else
			{
				RcDraw.Height = e->Y - RcDraw.Y;
			}

			//Get ROI image
			ROIImage = gcnew Bitmap(RcDraw.Width, RcDraw.Height);
			for(int y=0; y<RcDraw.Height; y++)
			{
				for(int x=0; x<RcDraw.Width; x++)
				{	
					Color RGB = openImage->GetPixel(x+RcDraw.X, y+RcDraw.Y); 
					ROIImage->SetPixel(x, y, Color::FromArgb(RGB.R, RGB.G, RGB.B));
				}
			}

			// Force a repaint of the region occupied by the rectangle...
			_draw = true;
			this->Invalidate(RcDraw);       
			this->Refresh();  
		}

private: System::Void pictureBox1_Paint(System::Object^  /*sender*/, System::Windows::Forms::PaintEventArgs^  e) {
			
			if(_draw)
			{
				// Draw the rectangle...
				float PenWidth = 1;
				e->Graphics->DrawRectangle( gcnew Pen( Color::Blue,PenWidth ), RcDraw );
			}
		 }

private: System::Void SelectCenterRegion(double* LevelSetResult, Bitmap ^ result){
			
			//result = gcnew Bitmap(ROIImage);

			int w, h;
			w = ROIImage->Width;
			h = ROIImage->Height;
			int *visit = (int *)malloc(w*h*sizeof(int));
			memset(visit, -1, w*h*sizeof(int));

			stack<int> xstack;
			stack<int> ystack;

			xstack.push(w/2);
			ystack.push(h/2);

			while(!xstack.empty()){

				int x = xstack.top();
				int y = ystack.top();
				xstack.pop();
				ystack.pop();

				if (x < 0 || x >= w)
					continue;
				if (y < 0 || y >= h)
					continue;

				int value = LevelSetResult[x*h+y];
				if (value > 0 || visit[x*h+y] != -1)
					continue;

				visit[x*h+y] = 1;
				result->SetPixel(x, y, Color::FromArgb(255, 255, 0));

				xstack.push(x+1);
				ystack.push(y);
				xstack.push(x-1);
				ystack.push(y);
				xstack.push(x);
				ystack.push(y+1);
				xstack.push(x);
				ystack.push(y-1);
			}

			/*for(int y=0; y<h; y++){
				for(int x=0; x<w; x++){
					int value = LevelSetResult[x*h+y];
					if (value <= 0)
						result->SetPixel(x, y, Color::FromArgb(255, 255, 0));	
				}
			}*/
			
}
		 

private: System::Void segmentationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			
			//Final Segmentation
			//Bitmap ^ showImage = gcnew Bitmap(openImage);
			/*for(int y=0; y<PreProImage->Height; y++){
				 for(int x=0; x<PreProImage->Width; x++){
					 Color color = PreProImage->GetPixel(x, y);
					 if((color.R+color.G+color.B)/3 > 15)
						 PreProImage->SetPixel(x, y, Color::FromArgb(255, 255, 255));
					 else
						 PreProImage->SetPixel(x, y, Color::FromArgb(0, 0, 0));
				 }
			 }*/
			 //pictureBox2->Image = PreProImage;

			//Bitmap ^ FinalSegmentation = ImageProcessingPtr->GraphCut(PreProImage, mask);
			PreProImage = ImageProcessingPtr->HistogramEqualization(PreProImage);
			//pictureBox2->Image = PreProImage;
			int w, h;
			w = PreProImage->Width;
			h = PreProImage->Height;
			int *PreproImageArray = (int *)malloc(w*h*sizeof(int));
			for(int y=0; y<h; y++){
				for(int x=0; x<w; x++){
					Color color = PreProImage->GetPixel(x, y);
					PreproImageArray[x*h+y] = (color.R+color.G+color.B)/3;
				}
			}
			
			chanvese ch = chanvese(PreproImageArray, w, h, _mask);
			double* result = ch.ChanVeseProcedure();

			Bitmap ^ ROIShowImage = gcnew Bitmap(ROIImage);;
			SelectCenterRegion(result, ROIShowImage);
			
			Bitmap ^ temp = gcnew Bitmap(ROIShowImage->Width, ROIShowImage->Height);
			for(int y=0; y<h; y++)
				for(int x=0; x<w; x++){
					Color RGB = ROIShowImage->GetPixel(x, y);
					if(RGB.R == 255 && RGB.G == 255 && RGB.B == 0)
						temp->SetPixel(x, y, Color::FromArgb(255, 255, 255));
					else
						temp->SetPixel(x, y, Color::FromArgb(0, 0, 0));
				}

			for(int y=1; y<ROIShowImage->Height-1; y++)
			{
				for(int x=1; x<ROIShowImage->Width-1; x++)
				{			
					int cnt = 0;
					for(int j=-1; j<=1; j++)
					{
						for(int i=-1; i<=1; i++)
						{
							Color RGB = temp->GetPixel(x+i, y+j); 
							if(RGB.R == 255 && RGB.G == 255 && RGB.B == 255)
								cnt++;
						}
					}
					
					if( cnt == 9 ){
						Color RGB = ROIImage->GetPixel(x, y);
						ROIShowImage->SetPixel(x, y, RGB);
					}
				}
			}

			pictureBox2->Image = ROIShowImage;
		 }

private: System::Void initialSegmentationToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			//Need to select ROI first.
			if(!_draw)
			{
				printf("Dosen't Select ROI!");
				return;
			}

			//Preprocessing (median filter -> anisotropy diffusion -> gramma correction)
			ROIImage = gcnew Bitmap(ROIImage, ROIImage->Width*2, ROIImage->Height*2);
			PreProImage = ImageProcessingPtr->Preprocessing(ROIImage);

			//Initial Segmentation (Dijkstras Algorithm)
			InitialSegmentation = ImageProcessingPtr->InitialSegmentation(PreProImage);
			
			//pictureBox2->Image =  ROIImage;
			pictureBox2->Image = InitialSegmentation;
		 }

private: System::Void seedToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

			//Seed for Final Segmentation
			mask = gcnew Bitmap(ROIImage, InitialSegmentation->Width, InitialSegmentation->Height);
			int minH = 10000000;
			int ox, oy;
			for(int x=0; x<InitialSegmentation->Width; x++)
			{
				int up, down;
				for(int y=0; y<InitialSegmentation->Height;y++)
				{	
					Color RGB = InitialSegmentation->GetPixel(x, y); 
					if(RGB.R == 255 && RGB.G == 0 && RGB.B == 0)
					{
						up = y;
					}
					if(RGB.R == 0 && RGB.G == 255 && RGB.B == 0)
					{
						down = y;
						break;
					}
				}
				
				if(x == InitialSegmentation->Width/2){
				//if(down-up < minH){
					minH = (down-up)/2;
					ox = x;
					oy = (up+down)/2;
					printf("%d %d %d\n", up, down, oy);
				}	
			}

			int w, h;
			w = InitialSegmentation->Width;
			h = InitialSegmentation->Height;
			_mask = (int *)malloc(w*h*sizeof(int));
			for(int y=0; y<h; y++){
				for(int x=0; x<w; x++){
					int dist = sqrt((x-ox)*(x-ox)+(y-oy)*(y-oy));
			
					if( dist == minH/2 )
						_mask[x*h+y] = 0;
					else if(dist > minH/2)
						_mask[x*h+y] = 1;
					else
						_mask[x*h+y] = -1;
				}
			}

			for(int y=0; y<mask->Height; y++){
				for(int x=0; x<mask->Width; x++){ 
					if( _mask[x*h+y] <= 0 )
						mask->SetPixel(x, y, Color::FromArgb(255, 255, 0));
				}
			}

			pictureBox2->Image = mask;
		 }
};
}
