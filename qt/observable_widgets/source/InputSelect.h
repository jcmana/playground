#pragma once

#include <QtWidgets/QComboBox>

#include "observable.hpp"
#include "input_select_model.hpp"

class InputSelect :
    public QComboBox
{
public:
    InputSelect(QWidget * parent = nullptr) :
        QComboBox(parent)
    {
        m_soState.observe(&InputSelect::OnModelChanged, this);

        connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &InputSelect::OnCurrentIndexChanged);
    }

    shared_obe<input_select_model> soState()
    {
        return m_soState;
    }

    shared_obe<const input_select_model> soState() const
    {
        return m_soState;
    }

private:
    void OnModelChanged(const input_select_model & state)
    {
        auto update = [this, state]
        {
            QSignalBlocker blocker(this);
            
            clear();
            for (auto n = 0; n < state.size(); ++n)
            {
                addItem(QString::fromUtf8(state.at(n).label.c_str()));

                if (state.at(n).selected)
                {
                    setCurrentIndex(n);
                }
            }
        };

        QMetaObject::invokeMethod(this, update);
    }

    void OnCurrentIndexChanged(int index)
    {
        unique_txn{m_soState}.get().select(static_cast<std::size_t>(index));
    }

private:
    shared_obe<input_select_model> m_soState;
};