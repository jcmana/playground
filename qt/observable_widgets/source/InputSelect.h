#pragma once

#include <QtWidgets/QComboBox>

#include "observable.hpp"
#include "input_select_state.hpp"

class InputSelect :
    public QComboBox
{
public:
    InputSelect(QWidget * parent = nullptr) :
        QComboBox(parent)
    {
        m_soState.observe(&InputSelect::OnStateChanged, this);

        connect(this, &QComboBox::currentIndexChanged, this, &InputSelect::OnCurrentIndexChanged);
    }

    shared_obe<input_select_state> soState()
    {
        return m_soState;
    }

    shared_obe<const input_select_state> soState() const
    {
        return m_soState;
    }

private:
    void OnStateChanged(const input_select_state & state)
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
    shared_obe<input_select_state> m_soState;
};