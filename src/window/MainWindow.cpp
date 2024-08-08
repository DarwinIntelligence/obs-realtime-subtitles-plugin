#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent): QDialog(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setVisible(false);
    setWindowTitle(QString::fromUtf8(T_WINDOW_TITLE));
    ui->audio_source_label->setText(QString::fromUtf8(T_AUDIO_SOURCE_LABEL));
    ui->source_language_label->setText(QString::fromUtf8(T_SOURCE_LANGUAGE_LABEL));
    ui->translate_language_label->setText(QString::fromUtf8(T_TRANSLATE_LANGUAGE_LABEL));
    ui->captions_view_label->setText(QString::fromUtf8(T_CAPTIONS_VIEW_LABEL));
    ui->profanity_filter_label->setText(QString::fromUtf8(T_PROFANITY_FILTER_LABEL));
    ui->output_to_label->setText(QString::fromUtf8(T_OUTPUT_TO_LABEL));
    ui->caption_timeout_label->setText(QString::fromUtf8(T_CAPTION_TIMEOUT_LABEL));
    ui->cancel_button->setText(QString::fromUtf8(T_CANCEL_BUTTON));
    ui->save_button->setText(QString::fromUtf8(T_SAVE_BUTTON));
}

void MainWindow::show_self()
{
    show();
    raise();
}

MainWindow::~MainWindow()
{

}
