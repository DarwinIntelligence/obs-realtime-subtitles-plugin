#include "MainWindow.h"


MainWindow::MainWindow(QWidget* parent): QDialog(parent), ui(new Ui::MainWindow)
{
    // Setup main window for the settings
    // Show UI and link the buttons and input to function callbacks
    ui->setupUi(this);
    setVisible(false);
    // Use title from locale
    setWindowTitle(QString::fromUtf8(T_WINDOW_TITLE));

    setupUIText();

    ConnectUISignalHandlers();
}

void MainWindow::setupUIText()
{
    // We have many locales so load up from constant obs module locale
    obs_log(LOG_INFO, "Setting up UI Field Labels");
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

void MainWindow::ConnectUISignalHandlers()
{
    QObject::connect(ui->cancel_button,
            SIGNAL(clicked()),
            this,
            SLOT(close()));

    QObject::connect(ui->save_button,
            SIGNAL(clicked()),
            this,
            SLOT(SaveButtonClicked()));

    QObject::connect(ui->audio_source_dropdown,
                    SIGNAL(currentTextChanged(QString)),
                    this,
                    SLOT(HandleAudioSourceChange(QString)));

    QObject::connect(ui->source_language_dropdown,
                    SIGNAL(currentTextChanged(QString)),
                    this,
                    SLOT(HandleSourceLanguageChange(QString)));

    QObject::connect(ui->translate_language_dropdown,
                    SIGNAL(currentTextChanged(QString)),
                    this,
                    SLOT(HandleTranslateLanguageChange(QString)));
}

void MainWindow::show_self()
{
    show();
    raise();
}

void MainWindow::HandleAudioSourceChange(QString new_text)
{
    std::string text = new_text.toStdString();
    obs_log(LOG_INFO, "New value selected for Audio Source");
}

void MainWindow::HandleSourceLanguageChange(QString new_text)
{
    std::string text = new_text.toStdString();
    obs_log(LOG_INFO, "New value selected for Source Language");
}

void MainWindow::HandleTranslateLanguageChange(QString new_text)
{
    std::string text = new_text.toStdString();
    obs_log(LOG_INFO, "New value selected for Translate Language");
}


void MainWindow::SaveButtonClicked()
{
    // If save occurs, save current settings and maybe restart plugin
    // data retrieval for STT + Translate;
    obs_log(LOG_DEBUG, "Save Button clicked");
    close();
}

MainWindow::~MainWindow()
{

}
