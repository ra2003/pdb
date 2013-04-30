#include "dlgsavescan.h"
#include "ui_dlgsavescan.h"
//
#include <QImageWriter>
#include <QTime>
#include <QMessageBox>

DlgSaveScan::DlgSaveScan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSaveScan)
{
    ui->setupUi(this);
    //
    QList<QByteArray> write_list = QImageWriter::supportedImageFormats();
    //
    QByteArray ba;
     foreach( ba, write_list )
     {
        QString str_ext(ba);
        ui->comboExtentions->addItem(str_ext);
     };
     //
     const QTime c_time = QTime::currentTime();
     const QString str_timestamp = c_time.toString("hh_mm_ss");
     QString str_full_path =QString("document_scan_%1").arg( str_timestamp );
     //
     ui->lineAttachmentName->setText(str_full_path);

     ui->comboExtentions->setCurrentIndex(0);
     //
     QObject::connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject() ));
     QObject::connect(ui->buttonSave,   SIGNAL(clicked()), this, SLOT(onSave() ));
     //
     ui->QualitySlider->setMinimum(10);
     ui->QualitySlider->setMaximum(100);
     ui->QualitySlider->setValue(100);
     //
     ui->lineAttachmentName->setFocus();
}

DlgSaveScan::~DlgSaveScan()
{
    delete ui;
}

void DlgSaveScan::onSave()
{
    m_iImageQuality     = ui->QualitySlider->value();
    m_strAttachmentName = ui->lineAttachmentName->text();
    m_strExtention      = ui->comboExtentions->currentText();
    //
    m_strAttachmentName = m_strAttachmentName.trimmed();
    //
    if (m_strAttachmentName.length() == 0)
    {
        QMessageBox::critical(NULL,
                                  trUtf8("Error!") ,
                                  "Attachment name can not be empty",
                                  QMessageBox::Ok,
                                  QMessageBox::Ok
                                            );
        ui->lineAttachmentName->setFocus();
        return;
    };
    //
    this->accept();
}
