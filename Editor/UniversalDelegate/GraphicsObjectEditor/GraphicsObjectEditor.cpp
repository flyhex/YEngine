#include "GraphicsObjectEditor.h"
#include <Editor/Editor/Editor.h>



GraphicsObjectEditor::GraphicsObjectEditor(QWidget* _parent): QWidget(_parent)
{
	standardFormat.setForeground(Qt::black);
	standardFormat.setFontPointSize(10);

	unknownArgFormat.setForeground(Qt::black);
	unknownArgFormat.setBackground(QColor(231,186,188));
	fileNameEdit = new StringLineEdit(this);
	fileNameEdit->setGeometry(QRect(0, 0, 90, 24));
	connect(fileNameEdit, SIGNAL(FocusOut()), this, SLOT(FocusOut()));
	connect(fileNameEdit, SIGNAL(textChanged(QString)), this, SLOT(NameIsChanged(QString)));
	openFileButton = new QToolButton(this);
	openFileButton->setGeometry(QRect(90, 0, 26, 26));
	openFileButton->setIcon(QPixmap(Editor::_GetWorkDir() + Editor::_GetResDir() + "TriggerCreator/finder.png"));
	openFileButton->setFixedSize(QSize(26, 26));
	openFileButton->setIconSize(QSize(26, 26));
	openFileButton->setToolTip("finder");
	connect(openFileButton, SIGNAL(clicked()), this, SLOT(OpenFinder()));
	editAssetButton = new QToolButton(this);
	editAssetButton->setGeometry(QRect(90+24-1-1, -1, 50+3, 24+2));
	editAssetButton->setText("create");
	editAssetButton->setEnabled(false);
}



void GraphicsObjectEditor::FocusOut(void)
{
	emit FocusOut(this);
}



void GraphicsObjectEditor::SetFileName(QString _fileName)
{
	fileNameEdit->setText(_fileName);
}


QString GraphicsObjectEditor::GetFileName(void)const
{
	return fileNameEdit->text();
}


void GraphicsObjectEditor::NameIsChanged(QString _name)
{
	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateObject()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditObject()));

	disconnect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditScene()));


	CheckArg();


	if(_name.isEmpty())
	{
		editAssetButton->setEnabled(false);
	}
	else
	{
		editAssetButton->setEnabled(true);
		bool exist = false;
		if(AssetLibrary::_IsAssetExist(_name.toStdString(), AssetLibrary::SCENE_ASSET))
		{
			exist = true;

			editAssetButton->setText("edit");
			connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditScene()));
		}
		else if(AssetLibrary::_IsAssetExist(_name.toStdString(), AssetLibrary::OBJECT_ASSET))
		{
			exist = true;

			editAssetButton->setText("review");
			connect(editAssetButton, SIGNAL(clicked()), this, SLOT(EditObject()));
		}

		if(!exist)
		{
			editAssetButton->setText("create");
			connect(editAssetButton, SIGNAL(clicked()), this, SLOT(CreateObject()));
		} 
	} 
}



void GraphicsObjectEditor::OpenFinder(void)
{
	QFileDialog dialog(this, "Choose graphics object");
	dialog.setFileMode(QFileDialog::ExistingFile);
	
	dialog.setNameFilter(QString(" *.") + AbstractObject::_GetFileSuffix().c_str() +
					     QString(" *.") + ObjectScene::_GetFileSuffix().c_str());

	dialog.setViewMode(QFileDialog::List);
	dialog.setDirectory(AssetLibrary::_GetDir().c_str());

	if(dialog.exec())
	{
		QStringList path = dialog.selectedFiles();
					
		if(path.size())
		{
			QFileInfo fileInfo(path.at(0));

			QString fileName = fileInfo.completeBaseName();
	
			fileNameEdit->setText(fileName);
		}
	}
}


void GraphicsObjectEditor::CheckArg(void)
{
	_ClearLineEditTextFormat(fileNameEdit);

	QList<QTextLayout::FormatRange> formats;
	QString string = fileNameEdit->text();
	int index = 0;

	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = string.length();
		format.format = standardFormat;
		formats.append(format);
	}
	int32 errorCode;
	if((errorCode = BooleanExprParser::_IsNameValid(string.toStdString())) >= 0)
	{
		QString text = "Undefined character(" + QString::number(errorCode + 1) + "): '" + QString(string.at(errorCode)) + "'";
		fileNameEdit->setToolTip(text);

		QTextLayout::FormatRange format;
		format.start = errorCode;
		format.length = 1;
		format.format = unknownArgFormat;
		formats.append(format);
	}
	else { fileNameEdit->setToolTip(""); } 


	index = 0;
	bool exist = false;
	if(AssetLibrary::_IsAssetExist(fileNameEdit->text().toStdString(), AssetLibrary::OBJECT_ASSET) ||
	   AssetLibrary::_IsAssetExist(fileNameEdit->text().toStdString(), AssetLibrary::SCENE_ASSET))
	{
		exist = true;
	}

	if(!exist)
	{
		QTextLayout::FormatRange format;
		format.start = index;
		format.length = string.length();
		format.format = unknownArgFormat;
		formats.append(format);

		if(errorCode < 0)
		{
			QString text = "Asset does not exist";
			fileNameEdit->setToolTip(text);
		}
	}
	else 
	{ 
		if(errorCode < 0)
		{
			fileNameEdit->setToolTip(""); 
		}
	} 


	_SetLineEditTextFormat(fileNameEdit, formats);
}





void GraphicsObjectEditor::EditObject(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(".") + AbstractObject::_GetFileSuffix().c_str() + QString(" review") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}


void GraphicsObjectEditor::CreateObject(void)
{  
	StringANSI name = fileNameEdit->text().toStdString();
	
	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));

	PROCESS_INFORMATION procInfo;
	if(CreateProcessA(QString("ObjectEditor.exe").toAscii().data(), (QString("ObjectEditor.exe ") + QString(name.c_str()) + QString(" new") + QString(" nameDisable")).toAscii().data(), NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		Editor::_GetProcesses().AddToTail(procInfo.hProcess);
	}
}




void GraphicsObjectEditor::_SetLineEditTextFormat(QLineEdit* _lineEdit, const QList<QTextLayout::FormatRange>& _formats)
{
    if(_lineEdit)
	{
		QList<QInputMethodEvent::Attribute> attributes;

		foreach(const QTextLayout::FormatRange& formatRange, _formats)
		{
			QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
			int start = formatRange.start - _lineEdit->cursorPosition();
			int length = formatRange.length;
			QVariant value = formatRange.format;
			attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
		}

		QInputMethodEvent event(QString(), attributes);
		QCoreApplication::sendEvent(_lineEdit, &event);
	}
}



void GraphicsObjectEditor::_ClearLineEditTextFormat(QLineEdit* _lineEdit)
{
    _SetLineEditTextFormat(_lineEdit, QList<QTextLayout::FormatRange>());
}




void GraphicsObjectEditor::resizeEvent(QResizeEvent* _event)
{
	fileNameEdit->setGeometry(QRect(0, 0, _event->size().width() - 26 - 50+2, 24));
	openFileButton->setGeometry(QRect(_event->size().width() - 26 - 50, -1, 26, 24));
	editAssetButton->setGeometry(QRect(_event->size().width() - 50-1-1, -1, 50+3, 24+2));
	QWidget::resizeEvent(_event);
}


