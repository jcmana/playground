#pragma once

#include <memory>
#include <vector>
#include <string>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtWidgets/QComboBox>

#include "observable.hpp"

namespace ComboBoxLogic
{

/*
class ComboBoxLogic
{
public:
    ComboBoxLogic(
        QComboBox * pComboBox,
        shared_obe<std::vector<std::string>> soItemList,
        shared_obe<std::string> soItemSelected);

private:
    

private:
    // Connection guard to disconnect QComboBox singals when this logic
    // object is destroyed
    std::unique_ptr<QObject> m_upConnectionGuard;

    QComboBox * m_pComboBox;

    shared_obe<std::vector<std::string>> m_soItemList;
    shared_obe<std::string> m_soItemSelected;
};
*/

class List
{
public:
    List(QComboBox * pComboBox, shared_obe<std::vector<std::string>> soList) :
        m_pComboBox(pComboBox),
        m_soList(soList)
    {
        m_upConnectionGuard = std::make_unique<QObject>();

        auto onChanged = [this](const auto & list)
        {
            QMetaObject::invokeMethod(m_upConnectionGuard.get(), std::bind(&List::UpdateComboBox, this, list));
        };
        m_soList.observe(onChanged);

        UpdateComboBox(shared_txn{m_soList});
    }

private:
    void UpdateComboBox(const std::vector<std::string> & list)
    {
        m_pComboBox->clear();
        for (const auto & item : list)
        {
            m_pComboBox->addItem(QString::fromUtf8(item.c_str()));
        };
    }

private:
    std::unique_ptr<QObject> m_upConnectionGuard;

    QComboBox * m_pComboBox;
    shared_obe<std::vector<std::string>> m_soList;
};

class Selection
{
public:
    Selection(QComboBox * pComboBox, shared_obe<std::optional<std::size_t>> soIndex) :
        m_pComboBox(pComboBox),
        m_soIndex(soIndex)
    {
        m_upConnectionGuard = std::make_unique<QObject>();

        auto onStateChanged = [this](const auto & index)
        {
            QMetaObject::invokeMethod(m_upConnectionGuard.get(), std::bind(&Selection::UpdateComboBox, this, index));
        };
        soIndex.observe(onStateChanged);

        auto onViewChanged = [this](int index)
        {
            unique_txn{m_soIndex} = index;
        };
        QObject::connect(m_pComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), m_upConnectionGuard.get(), onViewChanged);

        UpdateComboBox(shared_txn{m_soIndex});
    }

private:
    void UpdateComboBox(const std::optional<std::size_t> & index)
    {
        QSignalBlocker blocker(m_pComboBox);

        if (index.has_value())
        {
            m_pComboBox->setCurrentIndex(static_cast<int>(index.value()));
        }
        else
        {
            m_pComboBox->setCurrentIndex(-1);
        }
    }

private:
    std::unique_ptr<QObject> m_upConnectionGuard;

    QComboBox * m_pComboBox;
    shared_obe<std::optional<std::size_t>> m_soIndex;
};

class Repopulate
{
public:
    Repopulate(shared_obe<std::vector<std::string>> soList, shared_obe<std::optional<std::size_t>> soIndex) :
        m_soList(soList),
        m_soIndex(soIndex)
    {
        auto updateIndex = [this](const auto & list)
        {
            shared_txn txIndex{m_soIndex};

            if (txIndex.get().has_value() == false)
            {
                return;
            }

            if (list.empty())
            {
                //unique_txn<std::optional<std::size_t>>(std::move(txIndex)) = std::nullopt;
                return;
            }

            if (txIndex.get().value() >= list.size())
            {
                //unique_txn{txIndex} = 0;
                return;
            }

            if (txIndex.get().value() >= m_listPrevious.size())
            {
                //unique_txn{txIndex} = 0;
                return;
            }

            const auto valueOnIndexPrevious = m_listPrevious.at(txIndex.get().value());
            if (valueOnIndexPrevious != m_listPrevious.at(txIndex.get().value()))
            {
                //unique_txn{m_soIndex} = 0;
                return;
            }
        };
        m_soList.observe(updateIndex);

        auto updateListPrevious = [this](const auto & list)
        {
            m_listPrevious = list;
        };
        m_soList.observe(updateListPrevious);
    }

private:
    std::vector<std::string> m_listPrevious;

    shared_obe<std::vector<std::string>> m_soList;
    shared_obe<std::optional<std::size_t>> m_soIndex;
};

}