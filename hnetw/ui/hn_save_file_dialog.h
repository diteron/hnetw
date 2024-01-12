#pragma once

class HnSaveFileDialog : public QDialog {
public:
    HnSaveFileDialog(QWidget* parent = nullptr);
    ~HnSaveFileDialog();

    bool isDiscarded() const;
    int exec() override;

    enum Results {
        Accepted,
        Rejected,
        Discarded
    };

private:
    void addButtonBox();

    QLayout* layout_ = nullptr;
    QLabel* saveLabel_ = nullptr;
    QSpacerItem* spacer_ = nullptr;

    QDialogButtonBox* buttonBox_ = nullptr;
    QPushButton* saveButton_ = nullptr;
    QPushButton* dontSaveButton_ = nullptr;
    QPushButton* cancelButton_ = nullptr;
    bool isDiscarded_ = false;

public slots:
    void save();
    void dontSave();
    void discard();
};

