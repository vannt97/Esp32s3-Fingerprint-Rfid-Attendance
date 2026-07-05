#include "WifiManagerScreen.h"
#include "display/ScreenManager.h"
#include "utils/DisplayUtils.h"
WifiManagerScreen::WifiManagerScreen(ScreenManager &sm)
    : _screenManager(sm),
      _displayManager(sm.getDisplayManager())
{
}

// ── Public ────────────────────────────────────────────────

void WifiManagerScreen::onEnter()
{
    _selectedIdx = 0;
    _scrollOffset = 0;
    _doScan();
}

void WifiManagerScreen::onExit()
{
    _displayManager.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
}

void WifiManagerScreen::loop()
{
    if (_state != State::LIST)
        return;

    int count = _scanner.getCount();
    Button btn = _screenManager.getButtonManager().getPressed();

    switch (btn)
    {
    case Button::UP:
        if (_selectedIdx > 0)
        {
            _selectedIdx--;
            // scroll up nếu vượt ra ngoài window
            if (_selectedIdx < _scrollOffset)
                _scrollOffset = _selectedIdx;
            _render();
        }
        break;

    case Button::DOWN:
        if (_selectedIdx < count - 1)
        {
            _selectedIdx++;
            // scroll down nếu vượt ra ngoài window
            if (_selectedIdx >= _scrollOffset + VISIBLE_ROWS)
                _scrollOffset = _selectedIdx - VISIBLE_ROWS + 1;
            _render();
        }
        break;

    case Button::SELECT:
    {
        WifiNetwork net = _scanner.getNetwork(_selectedIdx);
        if (net.isOpen)
        {
            // TODO: _screenManager.showConnectingWifiScreen(net.ssid)
        }
        else
        {
            // TODO: showPasswordInputScreen(net.ssid)
        }
        break;
    }

    case Button::EXIT:
        _screenManager.showScreen(ScreenId::MENU);
        break;

    case Button::NONE:
        break;
    }
}

// ── Private ───────────────────────────────────────────────

void WifiManagerScreen::_doScan()
{
    _state = State::SCANNING;
    _renderScanning();

    int found = _scanner.scan();

    _state = (found > 0) ? State::LIST : State::EMPTY;
    _selectedIdx = 0;
    _scrollOffset = 0;
    _render();
}

void WifiManagerScreen::_render()
{
    switch (_state)
    {
    case State::SCANNING:
        _renderScanning();
        break;
    case State::LIST:
        _renderList();
        break;
    case State::EMPTY:
        _renderEmpty();
        break;
    }
}

void WifiManagerScreen::_renderScanning()
{
    _displayManager.clear();
    _displayManager.setTextColor(1);
    _displayManager.setTextWrap(false);
    _displayManager.print(10, 24, "Scanning WiFi...");
    _displayManager.update();
}

void WifiManagerScreen::_renderEmpty()
{
    _displayManager.clear();
    _displayManager.setTextColor(1);
    _displayManager.setTextWrap(false);
    _displayManager.print(10, 20, "No networks found");
    _displayManager.print(0, 53, "[SEL] retry  [EXIT] back");
    _displayManager.update();
}

void WifiManagerScreen::_renderList()
{
    _displayManager.clear();
    _displayManager.setTextColor(1);
    _displayManager.setTextWrap(false);

    // Header
    _displayManager.print(0, 0, "WiFi Networks");

    int count = _scanner.getCount();

    for (int i = 0; i < VISIBLE_ROWS; i++)
    {
        int netIdx = _scrollOffset + i;
        if (netIdx >= count)
            break;

        int y = LIST_Y + i * ROW_H;
        WifiNetwork net = _scanner.getNetwork(netIdx);

        // Highlight dòng được chọn
        if (netIdx == _selectedIdx)
            _displayManager.fillRect(0, y - 1, SCREEN_WIDTH, ROW_H, SSD1306_WHITE);

        int color = (netIdx == _selectedIdx) ? SSD1306_BLACK : SSD1306_WHITE;
        _displayManager.setTextColor(color);

        // Lock icon (dùng ký tự ASCII thay thế nếu không có bitmap)
        _displayManager.print(0, y, net.isOpen ? " " : "*");

        // SSID — truncate nếu quá dài
        String ssid = net.ssid;
        if (ssid.length() > 13)
            ssid = ssid.substring(0, 12) + "~";
        _displayManager.print(8, y, ssid);

        // RSSI bars (ví dụ: "|||" "||" "|")
        String barStr = rssiToBarString(net.rssi);
        _displayManager.print(104, y, barStr);
    }

    // Scrollbar đơn giản — chỉ hiện khi list dài hơn VISIBLE_ROWS
    if (count > VISIBLE_ROWS)
    {
        int barH = (VISIBLE_ROWS * (SCREEN_HEIGHT - LIST_Y)) / count;
        int barY = LIST_Y + (_scrollOffset * (SCREEN_HEIGHT - LIST_Y)) / count;
        _displayManager.fillRect(SCREEN_WIDTH - 2, barY, 2, barH, SSD1306_WHITE);
    }

    _displayManager.setTextColor(SSD1306_WHITE); // reset về default
    _displayManager.update();
}

