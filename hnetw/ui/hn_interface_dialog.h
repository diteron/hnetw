#pragma once

class HnInterfaceDialog : public QDialog {
public:
    HnInterfaceDialog(QWidget* parent = nullptr);
    ~HnInterfaceDialog();

    void setInterfacesIp(const std::vector<std::string>& ipStrings);
    int selectedInterfaceId();

    int exec() override;

private:
    void addChangeInterfaceRow();
    void addSpacerItem();
    void addButtonsBox();

    QFormLayout* layout_ = nullptr;
    QLabel* interfacesLabel_ = nullptr;
    QComboBox* interfacesCbox_ = nullptr;
    QSpacerItem* spacer_ = nullptr;

    QDialogButtonBox* buttons_ = nullptr;
    QPushButton* okButton_ = nullptr;
    QPushButton* cancelButton_ = nullptr;

    QStringList interfacesIp_;
    int selectedInterfaceId_ = 0;

private slots:
    void setInterfaceId();
};
