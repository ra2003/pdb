#include "dlgscanimage.h"
#include "ui_dlgscanimage.h"
#include "dlgrotateparams.h"
#include "dlgsavescan.h"
//
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QTransform>
#include <QBuffer>
//
DlgScanImage::DlgScanImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgScanImage)
{
    ui->setupUi(this);
    //
    m_ptrScanLabel  = NULL;
    m_ptrScanMode   = NULL;
    m_ptrScan       = NULL;
    //
    m_ptrRotate     = NULL;
    m_ptrCrop       = NULL;
    //
    m_ptrZoomIn     = NULL;
    m_ptrZoomOut    = NULL;
    m_ptrZoom_1_1   = NULL;
    //
    m_ptrWidthBox   = NULL;
    m_ptrHeightBox  = NULL;
    m_pResize       = NULL;
    //
    m_ptrSave       = NULL;
    m_ptrExit       = NULL;
    m_ptrToolBar    = NULL;
    m_ptrScrollArea = NULL;
    m_ptrImageLabel = NULL;
    //
    m_ptrImage      = NULL;
    //
    m_bIgnoreResize = true;
    //
    makeToolBar();
    setLinks();
    //
    const int i_screen_width = QApplication::desktop()->width();
    const int i_screen_height = QApplication::desktop()->height();
    //
    this->resize(i_screen_width/2, i_screen_height);
    //
    debugOpen();
    //
    int img_widh    = m_ptrImageLabel->width();
    int img_heigth  = m_ptrImageLabel->height();
    //
    double d_scale_factor = 1./std::max(((double)img_widh/(double)i_screen_width), ((double)img_heigth/(double)i_screen_height));

    scaleImage(d_scale_factor);


}

DlgScanImage::~DlgScanImage()
{
    delete ui;
    //
    if ( m_ptrScanLabel ) delete m_ptrScanLabel;
    if ( m_ptrScanMode  ) delete m_ptrScanMode;
    if ( m_ptrScan      ) delete m_ptrScan;
    if ( m_ptrRotate    ) delete m_ptrRotate;
    if ( m_ptrZoomIn    ) delete m_ptrZoomIn;
    if ( m_ptrZoomOut   ) delete m_ptrZoomOut;
    if ( m_ptrZoom_1_1  ) delete m_ptrZoom_1_1;
    //
    if ( m_ptrWidthBox  ) delete  m_ptrWidthBox;
    if ( m_ptrHeightBox ) delete  m_ptrHeightBox;
    if ( m_pResize      ) delete  m_pResize;
    //
    if ( m_ptrSave      ) delete m_ptrSave;
    if ( m_ptrExit      ) delete m_ptrExit;
    if ( m_ptrToolBar   ) delete m_ptrToolBar;
    //
    if (m_ptrImage      ) delete m_ptrImage;
    //if ( m_ptrScrollArea) delete m_ptrScrollArea;
    //if ( m_ptrImageLabel) delete m_ptrImageLabel;

}

void DlgScanImage::makeToolBar ()
{
    m_ptrScanLabel = new QLabel("Choose predefined configuration: ");
    //
    m_ptrScanMode   = new QComboBox(this);
    m_ptrScanMode   ->addItem("400 DPI, Full color ");
    m_ptrScanMode   ->addItem("200 DPI, Full color");
    m_ptrScanMode   ->addItem("300 DPI, Grayscale");
    //
    m_ptrScan       = new QAction(tr("Scan..."), this);
    m_ptrScan       ->setIconVisibleInMenu(true);
    m_ptrScan       ->setIcon(QIcon(":/images/images/scanner.png"));
    //m_ptrDirectScan    ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_O));
    m_ptrScan       ->setStatusTip(tr("Scan new document with selected scanner settings"));
    //
    m_ptrRotate       = new QAction(tr("Rotate"), this);
    m_ptrRotate       ->setIconVisibleInMenu(true);
    m_ptrRotate       ->setIcon(QIcon(":/images/images/rotate.png"));
    //m_ptrRotate    ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_O));
    m_ptrRotate       ->setStatusTip(tr("Rotate image on defined angle"));
    //
    m_ptrZoomIn     = new QAction(tr("Zoom in"), this);
    m_ptrZoomIn     ->setIconVisibleInMenu(true);
    m_ptrZoomIn     ->setIcon(QIcon(":/images/images/zoom_in.png"));
    m_ptrZoomIn     ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Plus));
    m_ptrZoomIn     ->setStatusTip(tr("Zoom in"));
    m_ptrZoomIn     ->setEnabled(true);
    //
    m_ptrZoomOut    = new QAction(tr("Zoom out"), this);
    m_ptrZoomOut    ->setIconVisibleInMenu(true);
    m_ptrZoomOut    ->setIcon(QIcon(":/images/images/zoom_out.png"));
    m_ptrZoomOut    ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Minus));
    m_ptrZoomOut    ->setStatusTip(tr("Zoom out"));
    m_ptrZoomOut    ->setEnabled(true);
    //
    m_ptrZoom_1_1   = new QAction(tr("Zoom 1:1"), this);
    m_ptrZoom_1_1   ->setIconVisibleInMenu(true);
    m_ptrZoom_1_1   ->setIcon(QIcon(":/images/images/zoom_1_1.png"));
    //m_ptrZoom_1_1   ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Minus));
    m_ptrZoom_1_1   ->setStatusTip(tr("Zoom 1:1"));
    m_ptrZoom_1_1   ->setEnabled(true);
    //
    m_ptrCrop   = new QAction(tr("Crop to selection"), this);
    m_ptrCrop   ->setIconVisibleInMenu(true);
    m_ptrCrop   ->setIcon(QIcon(":/images/images/crop.png"));
            //m_ptrZoom_1_1   ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Minus));
    m_ptrCrop   ->setStatusTip(tr("Crop image to selection"));
    m_ptrCrop   ->setEnabled(false);

    //
    m_ptrWidthLabel = new QLabel(" W: ");
    m_ptrWidthBox   = new QSpinBox();
    //
    m_ptrHeightLabel = new QLabel(" H: ");
    m_ptrHeightBox  = new QSpinBox();
    //
    m_pResize   = new QPushButton("Resize");
    m_pResize   ->setEnabled(false);
    //
    m_ptrSave       = new QAction(tr("Save image as attachment"), this);
    m_ptrSave       ->setToolTip(tr("Save image as attachment"));
    m_ptrSave       ->setIconVisibleInMenu(true);
    m_ptrSave       ->setIcon(QIcon(":/images/images/save_as.png"));
    m_ptrSave       ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_S));
    m_ptrSave       ->setStatusTip(tr("Save image as attachment"));
    m_ptrSave       ->setEnabled(true);
    //
    m_ptrExit       = new QAction(tr("Exit"), this);
    m_ptrExit       ->setIcon(QIcon(":/images/images/exit.png"));
    m_ptrExit       ->setShortcut( QKeySequence (Qt::CTRL + Qt::Key_Q) );
    m_ptrExit       ->setStatusTip(tr("Exit"));
    m_ptrExit       ->setEnabled(true);
    //
    m_ptrToolBar    = new QToolBar("Options", this);
    m_ptrToolBar   ->addWidget(m_ptrScanLabel);
    m_ptrToolBar   ->addWidget(m_ptrScanMode);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addAction(m_ptrScan);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addAction(m_ptrRotate);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addAction(m_ptrZoomIn);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addAction(m_ptrZoomOut);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addAction(m_ptrZoom_1_1);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addAction(m_ptrCrop);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addWidget(m_ptrWidthLabel);
    m_ptrToolBar   ->addWidget(m_ptrWidthBox);
    m_ptrToolBar   ->addWidget(m_ptrHeightLabel);
    m_ptrToolBar   ->addWidget(m_ptrHeightBox);
    m_ptrToolBar   ->addWidget(m_pResize);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addAction(m_ptrSave);
    m_ptrToolBar   ->addSeparator();
    m_ptrToolBar   ->addAction(m_ptrExit);
    //
    m_ptrImageLabel = new ImageLabel;
    m_ptrImageLabel->setBackgroundRole(QPalette::Base);
    m_ptrImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_ptrImageLabel->setScaledContents(true);
    //
    m_ptrScrollArea = new QScrollArea;
    m_ptrScrollArea->setBackgroundRole(QPalette::Dark);
    m_ptrScrollArea->setWidget(m_ptrImageLabel);
    //
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(m_ptrToolBar,Qt::AlignTop);
    layout->addWidget(m_ptrScrollArea,Qt::AlignBottom);
}

void DlgScanImage::debugOpen ()
{
/*
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
*/
    //this is DEBUG code
    const QString fileName = "/home/alex/Fun/Autum/darktable_exported/img_test.jpg";
    m_ptrImage = new QPixmap(fileName);
/*
    if (!fileName.isEmpty())
    {
*/
//        QImage image(fileName);
        if (m_ptrImage->isNull())
        {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        m_ptrImageLabel->setPixmap(*m_ptrImage);
        m_dscaleFactor = 1.0;

        m_ptrWidthBox->setMaximum(m_ptrImage->width());
        m_ptrWidthBox->setValue(m_ptrImage->width());
        //
        m_d_WH_Ratio = (double) m_ptrImage->width() / (double) m_ptrImage->height();
        //
        m_ptrHeightBox->setMaximum(m_ptrImage->height());
        m_ptrHeightBox->setValue(m_ptrImage->height());
        //
        m_ptrImageLabel->adjustSize();
        //
        m_bIgnoreResize = false;
}

void DlgScanImage::setLinks ()
{
    QObject::connect(m_ptrExit,         SIGNAL(triggered()),        this, SLOT(reject()             ));
    //
    QObject::connect(m_ptrRotate,       SIGNAL(triggered()),        this, SLOT(onRotate()           ));
    //
    QObject::connect(m_ptrZoomIn,       SIGNAL(triggered()),        this, SLOT(onZoomIn()           ));
    QObject::connect(m_ptrZoomOut,      SIGNAL(triggered()),        this, SLOT(onZoomOut()          ));
    QObject::connect(m_ptrZoom_1_1,     SIGNAL(triggered()),        this, SLOT(onZoom_1_1()         ));
    //
    QObject::connect(m_ptrCrop,         SIGNAL(triggered()),        this, SLOT(onCrop()             ));
    //
    QObject::connect(m_ptrWidthBox,     SIGNAL(valueChanged(int)),  this, SLOT(onWidthChanged(int)  ));
    QObject::connect(m_ptrHeightBox,    SIGNAL(valueChanged(int)),  this, SLOT(onHeightChanged(int) ));
    QObject::connect(m_pResize,         SIGNAL(clicked()),          this, SLOT(onResize()           ));
    //
    QObject::connect(m_ptrSave,         SIGNAL(triggered()),        this, SLOT(onSave()             ));
    //
    QObject::connect(m_ptrImageLabel,   SIGNAL(CropAllowed(bool)),  this, SLOT(onCropAllowed(bool)  ));
}

void DlgScanImage::onCrop ()
{
    //Not finished yet
    const QRect* ptr_rect  = m_ptrImageLabel->getSelection();
    if (NULL == ptr_rect)
        return;
    //
    QRect copy_rect = *ptr_rect;
    //
    m_ptrImageLabel->dropSelection();
    //
    copy_rect.setLeft  ( copy_rect.left()  / m_dscaleFactor );
    copy_rect.setRight ( copy_rect.right() / m_dscaleFactor );
    copy_rect.setTop   ( copy_rect.top()   / m_dscaleFactor );
    copy_rect.setBottom( copy_rect.bottom()/ m_dscaleFactor );
    //
    QPixmap* ptr_tmp = new QPixmap (m_ptrImage->copy(copy_rect));
    delete m_ptrImage;
    m_ptrImage = ptr_tmp;
    m_ptrImageLabel->setPixmap(*m_ptrImage);
    m_dscaleFactor = 1.0;
    m_ptrImageLabel->adjustSize();
    //
    m_bIgnoreResize = true;
    m_ptrHeightBox->setMaximum(m_ptrImage->height());
    m_ptrHeightBox->setValue(m_ptrImage->height());
    m_bIgnoreResize = true;
    m_ptrWidthBox->setMaximum(m_ptrImage->width());
    m_ptrWidthBox->setValue(m_ptrImage->width());
}

void DlgScanImage::onCropAllowed(bool b)
{
    if (m_ptrCrop)
        m_ptrCrop->setEnabled(b);
}

void DlgScanImage::onRotate ()
{
    DlgRotateParams dlg;
    //
    if (dlg.exec() != QDialog::Accepted)
        return;
    //
    bool b_is_clockwise = dlg.isClockWise();
    int i_angle = dlg.getAngle();
    //
    if (false == b_is_clockwise)
        i_angle *= -1;
    //
    QTransform transform;
    QTransform trans = transform.rotate(i_angle);
    //
    QPixmap* ptr_pixmap_tmp = new QPixmap(m_ptrImage->transformed(trans));
    delete m_ptrImage;
    m_ptrImage = ptr_pixmap_tmp;
    m_ptrImageLabel->setPixmap(*m_ptrImage);
    m_ptrImageLabel->adjustSize();
    m_dscaleFactor = 1.;
}

void DlgScanImage::onResize()
{
    m_pResize   ->setEnabled(false);
    //
    m_ptrWidthBox->setMaximum( m_ptrWidthBox->value() );
    m_ptrHeightBox->setMaximum( m_ptrHeightBox->value() );
    //
    QPixmap* ptr_tmp_image = new QPixmap (m_ptrImage->scaled(m_ptrWidthBox->value(), m_ptrHeightBox->value(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    //
    if (ptr_tmp_image->isNull())
    {
        Q_ASSERT(FALSE);
        delete ptr_tmp_image;
        return;
    };
    //
    delete m_ptrImage;
    m_ptrImage = ptr_tmp_image;
    m_ptrImageLabel->setPixmap(*m_ptrImage);
    m_ptrImageLabel->adjustSize();
    m_dscaleFactor = 1.;
}

void DlgScanImage::onWidthChanged(int i_width)
{
    if (false == m_bIgnoreResize)
    {
        m_bIgnoreResize = true;
        int i_new_height = i_width / m_d_WH_Ratio;
        m_ptrHeightBox->setValue(i_new_height);
        m_pResize   ->setEnabled(true);
        m_bIgnoreResize = false;
    };
}

void DlgScanImage::onHeightChanged(int i_height)
{
    if (false == m_bIgnoreResize)
    {
        m_bIgnoreResize = true;
        int i_new_width = i_height * m_d_WH_Ratio;
        m_ptrWidthBox->setValue(i_new_width);
        m_pResize   ->setEnabled(true);
        m_bIgnoreResize = false;
    };
}

void DlgScanImage::onSave()
{
    DlgSaveScan dlg;
    //
    if (dlg.exec() != QDialog::Accepted)
        return;
    //
    QBuffer buffer(&m_baImageAsArray); //( (const char *) ptr_img->bits(), ptr_img->numBytes() );
    buffer.open(QIODevice::WriteOnly);
    //
    bool b_save =  m_ptrImage->save(&buffer, dlg.getAttachmentExt().toStdString().c_str(), dlg.getImageQuality());
    m_strImageName = dlg.getAttachmentName();
    //
    Q_ASSERT (b_save);
    //
    if (false == b_save)
    {
        QString str_message = "Can not save attachment '";
        str_message += m_strImageName;
        str_message += "' in ";
        str_message += dlg.getAttachmentExt();
        str_message += " format";
        //
        QMessageBox::critical(NULL,
                                  trUtf8("Error!") ,
                                  str_message,
                                  QMessageBox::Ok,
                                  QMessageBox::Ok
                                            );
        return;
    };
    //
    this->accepted();
}


void DlgScanImage::onZoomIn   ()
{
    scaleImage(1.25);
}

void DlgScanImage::onZoomOut  ()
{
    scaleImage(0.8);
}

void DlgScanImage::onZoom_1_1 ()
{
    m_dscaleFactor = 1.;
    scaleImage(m_dscaleFactor);
}

void DlgScanImage::scaleImage  (double factor)
{
    Q_ASSERT(m_ptrImageLabel->pixmap());
    m_dscaleFactor *= factor;
    m_ptrImageLabel->resize(m_dscaleFactor * m_ptrImageLabel->pixmap()->size());

    adjustScrollBar(m_ptrScrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(m_ptrScrollArea->verticalScrollBar(), factor);

    m_ptrZoomIn->setEnabled(m_dscaleFactor < 7.0);
    m_ptrZoomOut->setEnabled(m_dscaleFactor > 0.05);
}

void DlgScanImage::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
