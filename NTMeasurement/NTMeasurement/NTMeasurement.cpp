// NTMeasurement.cpp: �D�n�M���ɡC

#include "stdafx.h"
#include "MyForm.h"
#include <iostream>

using namespace std;
using namespace System;
using namespace NTMeasurement;

[STAThread]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MyForm myForm;
	Application::Run(%myForm);
    return 0;
}
