#include <QtCore/QString>
#include <QtCore/QSignalBlocker>

#include "ComboBoxLogic.h"

namespace NonIntrusive
{

/*
ComboBoxLogic::ComboBoxLogic(
    QComboBox * pComboBox,
    shared_obe<std::vector<std::string>> soItemList,
    shared_obe<std::string> soItemSelected) :
    m_pComboBox(pComboBox),
    m_soItemList(soItemList),
    m_soItemSelected(soItemSelected)
{
    m_upConnectionGuard = std::make_unique<QObject>();

    auto itemListChanged = [this](const auto & itemList)
    {
        QSignalBlocker blocker(m_pComboBox);
        m_pComboBox->clear();

        for (const auto & item : itemList)
        {
            m_pComboBox->addItem(QString::fromUtf8(item.c_str()));
        }
    };
    m_soItemList.observe(itemListChanged);

    auto itemSelectionChanged = [this](const auto & item)
    {
        QSignalBlocker blocker(m_pComboBox);
        m_pComboBox->setCurrentText(QString::fromUtf8(item.c_str()));
    };
    m_soItemSelected.observe(itemSelectionChanged);

    auto comboBoxCurrentIndexChanged = [this](int index)
    {
        unique_txn{m_soItemSelected} = shared_txn{m_soItemList}.get().at(index);
    };
    QObject::connect(
        m_pComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
        m_upConnectionGuard.get(), comboBoxCurrentIndexChanged);

    // Load values from states
    itemListChanged(shared_txn{m_soItemList}.get());
    itemSelectionChanged(shared_txn{m_soItemSelected}.get());
}
*/

}