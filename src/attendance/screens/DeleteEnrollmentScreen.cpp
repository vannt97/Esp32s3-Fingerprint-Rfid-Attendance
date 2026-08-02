#include "DeleteEnrollmentScreen.h"
#include "display/ScreenManager.h"

DeleteEnrollmentScreen::DeleteEnrollmentScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

void DeleteEnrollmentScreen::onEnter()
{
    _selectedIndex = 0;
    _windowStart = 0;
    _mode = Mode::LIST;
    _buildList();
    _render();
}

void DeleteEnrollmentScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void DeleteEnrollmentScreen::_buildList()
{
    uint16_t employeeId = _screenManager.getSelectedEmployeeId();
    auto &store = _screenManager.getEnrollmentStore();

    _itemCount = 0;
    for (int i = 0; i < FINGER_COUNT; i++)
    {
        uint16_t templateId;
        if (store.getFingerMapping(employeeId, i, templateId))
        {
            _items[_itemCount].isCard = false;
            _items[_itemCount].fingerPosition = i;
            _items[_itemCount].templateId = templateId;
            _itemCount++;
        }
    }

    String uid;
    if (store.getCardMapping(employeeId, uid))
    {
        _items[_itemCount].isCard = true;
        _itemCount++;
    }

    if (_selectedIndex >= _itemCount)
        _selectedIndex = _itemCount > 0 ? _itemCount - 1 : 0;
    if (_windowStart > _selectedIndex)
        _windowStart = _selectedIndex;
}

void DeleteEnrollmentScreen::_render()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    _displayManager.print(0, 0, "Delete");
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    if (_itemCount == 0)
    {
        _displayManager.print(4, 24, "Nothing enrolled");
        _displayManager.print(89, 53, "Exit");
        _displayManager.update();
        return;
    }

    for (int i = 0; i < VISIBLE_COUNT; i++)
    {
        int itemIndex = _windowStart + i;
        if (itemIndex >= _itemCount)
            break;

        int y = ITEM_Y0 + i * ITEM_HEIGHT;
        if (itemIndex == _selectedIndex)
        {
            _displayManager.fillRect(0, y - 1, 120, ITEM_HEIGHT - 1, SSD1306_WHITE);
            _displayManager.setTextColor(SSD1306_BLACK);
        }
        else
        {
            _displayManager.setTextColor(SSD1306_WHITE);
        }

        const Item &item = _items[itemIndex];
        _displayManager.print(4, y, item.isCard ? "Card" : FINGER_NAMES[item.fingerPosition]);
    }

    _displayManager.setTextColor(SSD1306_WHITE);
    _drawScrollbar();

    _displayManager.print(13, 53, "Select");
    _displayManager.print(89, 53, "Exit");
    _displayManager.update();
}

void DeleteEnrollmentScreen::_drawScrollbar()
{
    const int trackX = 125;
    const int trackY = ITEM_Y0;
    const int trackH  = VISIBLE_COUNT * ITEM_HEIGHT - 1;

    _displayManager.drawLine(trackX, trackY, trackX, trackY + trackH, SSD1306_WHITE);

    int thumbH = trackH * VISIBLE_COUNT / _itemCount;
    if (thumbH < 3) thumbH = 3;

    int maxStart = _itemCount - VISIBLE_COUNT;
    int thumbY = trackY;
    if (maxStart > 0)
        thumbY = trackY + (trackH - thumbH) * _windowStart / maxStart;

    _displayManager.fillRect(trackX - 1, thumbY, 3, thumbH, SSD1306_WHITE);
}

void DeleteEnrollmentScreen::_renderConfirm()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    _displayManager.print(0, 0, "Delete");
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    const Item &item = _items[_selectedIndex];
    _displayManager.print(4, 24, "Delete:");
    _displayManager.print(4, 36, item.isCard ? "Card" : FINGER_NAMES[item.fingerPosition]);

    _displayManager.print(13, 53, "Yes");
    _displayManager.print(89, 53, "No");
    _displayManager.update();
}

void DeleteEnrollmentScreen::_renderDone()
{
    _displayManager.clear();
    _displayManager.setTextColor(SSD1306_WHITE);
    _displayManager.setTextWrap(false);

    _displayManager.print(0, 0, "Delete");
    _displayManager.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
    _displayManager.print(4, 24, "Deleted!");
    _displayManager.update();
}

void DeleteEnrollmentScreen::_confirmDelete()
{
    uint16_t employeeId = _screenManager.getSelectedEmployeeId();
    const Item &item = _items[_selectedIndex];

    if (item.isCard)
    {
        _screenManager.getEnrollmentStore().removeCardMapping(employeeId);
    }
    else
    {
        uint16_t deletedTemplateId;
        if (_screenManager.getEnrollmentStore().removeFingerMapping(employeeId, item.fingerPosition, deletedTemplateId))
            _screenManager.getFingerprintService().deleteTemplate(deletedTemplateId);
    }

    _mode = Mode::DONE;
    _renderDone();
}

void DeleteEnrollmentScreen::loop()
{
    Button btn = _screenManager.getButtonManager().getPressed();

    if (_mode == Mode::DONE)
    {
        if (btn == Button::SELECT || btn == Button::EXIT)
        {
            _buildList();
            _mode = Mode::LIST;
            _render();
        }
        return;
    }

    if (_mode == Mode::CONFIRM)
    {
        if (btn == Button::SELECT)
        {
            _confirmDelete();
        }
        else if (btn == Button::EXIT)
        {
            _mode = Mode::LIST;
            _render();
        }
        return;
    }

    // Mode::LIST
    switch (btn)
    {
    case Button::UP:
        if (_selectedIndex > 0)
        {
            _selectedIndex--;
            if (_selectedIndex < _windowStart)
                _windowStart--;
            _render();
        }
        break;
    case Button::DOWN:
        if (_selectedIndex < _itemCount - 1)
        {
            _selectedIndex++;
            if (_selectedIndex >= _windowStart + VISIBLE_COUNT)
                _windowStart++;
            _render();
        }
        break;
    case Button::SELECT:
        if (_itemCount > 0)
        {
            _mode = Mode::CONFIRM;
            _renderConfirm();
        }
        break;
    case Button::EXIT:
        _screenManager.showScreen(ScreenId::EMPLOYEE_SELECT);
        break;
    default:
        break;
    }
}