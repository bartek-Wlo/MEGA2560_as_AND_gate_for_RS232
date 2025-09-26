using System;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Linq;

namespace SerialTerminalApp
{
    public partial class Form1 : Form
    {
        private SerialPort? _serialPort;
        
        // Zmienne stanu dla maszyny routującej
        private readonly StringBuilder _lineBuffer = new StringBuilder();
        private RichTextBox? _currentRoutingTarget;
        private bool _isNewLine = true;

        private const string SendDataPlaceholder = "ASCII, np. 123456 to HEX 0x12 0x34 0x56";
        private const string DistancePlaceholder = "Wprowadź odległość (0-64)";

        public Form1()
        {
            InitializeComponent();
            this.WindowState = FormWindowState.Maximized;
            InitializeCustomComponents();
            LoadAvailableComPorts();
            UpdateControlsState(false);
        }

        private void InitializeCustomComponents()
        {
            var monoFont = new Font("Consolas", 9F, FontStyle.Regular, GraphicsUnit.Point, 0);
            rtbRawLog.Font = monoFont;
            rtbLog00.Font = monoFont;
            rtbLog1.Font = monoFont;
            rtbLog2.Font = monoFont;
            rtbLog3.Font = monoFont;
            rtbLog4.Font = monoFont;

            txtSendData.KeyDown += TxtSendData_KeyDown;
            txtDistance.KeyDown += TxtDistance_KeyDown;

            SetupPlaceholderText(txtSendData, SendDataPlaceholder);
            SetupPlaceholderText(txtDistance, DistancePlaceholder);
        }

        private void SetupPlaceholderText(TextBox textBox, string placeholder)
        {
            textBox.Text = placeholder;
            textBox.ForeColor = Color.Gray;
            textBox.GotFocus += (sender, e) => RemovePlaceholder(textBox, placeholder);
            textBox.LostFocus += (sender, e) => AddPlaceholder(textBox, placeholder);
        }

        private void RemovePlaceholder(TextBox textBox, string placeholder)
        {
            if (textBox.Text == placeholder)
            {
                textBox.Text = "";
                textBox.ForeColor = Color.Black;
            }
        }

        private void AddPlaceholder(TextBox textBox, string placeholder)
        {
            if (string.IsNullOrWhiteSpace(textBox.Text))
            {
                textBox.Text = placeholder;
                textBox.ForeColor = Color.Gray;
            }
        }

        private void LoadAvailableComPorts()
        {
            cmbComPorts.Items.Clear();
            string[] portNames = SerialPort.GetPortNames();
            if (portNames.Length > 0)
            {
                cmbComPorts.Items.AddRange(portNames);
                cmbComPorts.SelectedIndex = 0;
                btnConnect.Enabled = true;
            }
            else
            {
                cmbComPorts.Items.Add("Brak portów");
                cmbComPorts.SelectedIndex = 0;
                btnConnect.Enabled = false;
            }
        }

        private void UpdateControlsState(bool isConnected)
        {
            btnConnect.Text = isConnected ? "Rozłącz" : "Połącz";
            cmbComPorts.Enabled = !isConnected;
            btnRefreshPorts.Enabled = !isConnected;

            btnSnifMode.Enabled = isConnected;
            btnAndGateMode.Enabled = isConnected;
            btnToggleSending.Enabled = isConnected;
            txtSendData.Enabled = isConnected;
            btnManualSend.Enabled = isConnected;
            txtDistance.Enabled = isConnected;
            btnSetDistance.Enabled = isConnected;

            btnSaveLog.Enabled = true;
            btnClearLogs.Enabled = true;
        }

        private void AppendTextToRichTextBox(RichTextBox rtb, string text, Color color)
        {
            if (rtb.InvokeRequired)
            {
                rtb.Invoke(new Action(() => AppendTextToRichTextBox(rtb, text, color)));
                return;
            }
            rtb.SelectionStart = rtb.TextLength;
            rtb.SelectionLength = 0;
            rtb.SelectionColor = color;
            rtb.AppendText(text);
            rtb.SelectionColor = rtb.ForeColor; // Reset to default
            rtb.ScrollToCaret();
        }

        private void LogSystemMessage(string message)
        {
            string timestamp = $"[{DateTime.Now:HH:mm:ss.fff}] ";
            AppendTextToRichTextBox(rtbLog00, timestamp, Color.Gray);
            AppendTextToRichTextBox(rtbLog00, message + Environment.NewLine, rtbLog00.ForeColor);
        }

        private void SendData(string data)
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                try
                {
                    string timestamp = $"[{DateTime.Now:HH:mm:ss.fff}] ";
                    string message = $"WYSŁANO: {data}";
                    
                    AppendTextToRichTextBox(rtbRawLog, Environment.NewLine, rtbRawLog.ForeColor); // nowa linia przed wysłaniem
                    AppendTextToRichTextBox(rtbRawLog, timestamp, Color.Gray);
                    AppendTextToRichTextBox(rtbRawLog, message, Color.Gray);
                    
                    _serialPort.Write(data);
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Błąd podczas wysyłania danych: {ex.Message}", "Błąd", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
        
        private void btnRefreshPorts_Click(object sender, EventArgs e)
        {
            LoadAvailableComPorts();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.Close();
                UpdateControlsState(false);
                LogSystemMessage("Rozłączono z portem.");
            }
            else
            {
                if (cmbComPorts.SelectedItem == null || cmbComPorts.SelectedItem.ToString() == "Brak portów")
                {
                    MessageBox.Show("Wybierz prawidłowy port COM.", "Informacja", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return;
                }
                try
                {
                    _serialPort = new SerialPort(cmbComPorts.SelectedItem.ToString())
                    {
                        BaudRate = 115200, Parity = Parity.None, DataBits = 8, StopBits = StopBits.One
                    };
                    _serialPort.DataReceived += SerialPort_DataReceived;
                    _serialPort.Open();
                    UpdateControlsState(true);
                    LogSystemMessage($"Połączono z portem {_serialPort.PortName}.");
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Nie udało się otworzyć portu: {ex.Message}", "Błąd Połączenia", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (_serialPort == null || !_serialPort.IsOpen) return;
            try
            {
                string data = _serialPort.ReadExisting();
                this.BeginInvoke(new Action(() => ProcessReceivedData(data)));
            }
            catch (Exception) { /* Ignoruj błędy podczas zamykania */ }
        }

        private void ProcessReceivedData(string data)
        {
            foreach (char c in data)
            {
                // Zawsze loguj surowe dane
                if (_isNewLine)
                {
                    string timestamp = $"[{DateTime.Now:HH:mm:ss.fff}] ";
                    AppendTextToRichTextBox(rtbRawLog, timestamp, Color.Gray);
                }
                AppendTextToRichTextBox(rtbRawLog, c.ToString(), rtbRawLog.ForeColor);

                // Logika maszyny stanów
                if (c == '\n')
                {
                    if (_currentRoutingTarget != null)
                    {
                        AppendTextToRichTextBox(_currentRoutingTarget, Environment.NewLine, _currentRoutingTarget.ForeColor);
                    }
                    else // Jeśli linia kończy się zanim znaleziono cel, trafia do rtbLog00
                    {
                        if (_isNewLine)
                        {
                            string timestamp = $"[{DateTime.Now:HH:mm:ss.fff}] ";
                            AppendTextToRichTextBox(rtbLog00, timestamp, Color.Gray);
                        }
                        AppendTextToRichTextBox(rtbLog00, _lineBuffer.ToString() + Environment.NewLine, rtbLog00.ForeColor);
                    }
                    // Reset stanu
                    _lineBuffer.Clear();
                    _currentRoutingTarget = null;
                    _isNewLine = true;
                    continue;
                }

                _isNewLine = false;

                if (_currentRoutingTarget != null)
                {
                    AppendTextToRichTextBox(_currentRoutingTarget, c.ToString(), _currentRoutingTarget.ForeColor);
                }
                else
                {
                    _lineBuffer.Append(c);
                    string currentLine = _lineBuffer.ToString();
                    RichTextBox? foundTarget = null;

                    if (currentLine.Contains("01:")) foundTarget = rtbLog1;
                    else if (currentLine.Contains("02:")) foundTarget = rtbLog2;
                    else if (currentLine.Contains("03:")) foundTarget = rtbLog3;
                    else if (currentLine.Contains("04:")) foundTarget = rtbLog4;
                    else if (currentLine.Contains("00:")) foundTarget = rtbLog00;

                    if (foundTarget != null)
                    {
                        _currentRoutingTarget = foundTarget;
                        string timestamp = $"[{DateTime.Now:HH:mm:ss.fff}] ";
                        AppendTextToRichTextBox(_currentRoutingTarget, timestamp, Color.Gray);
                        AppendTextToRichTextBox(_currentRoutingTarget, currentLine, _currentRoutingTarget.ForeColor);
                        _lineBuffer.Clear();
                    }
                }
            }
        }
        
        private void btnSnifMode_Click(object sender, EventArgs e) => SendData("55330FCC");
        private void btnAndGateMode_Click(object sender, EventArgs e) => SendData("5533F0CC");
        private void btnToggleSending_Click(object sender, EventArgs e) => SendData("5533EECC");

        private void btnManualSend_Click(object sender, EventArgs e)
        {
            string textToSend = txtSendData.Text.Trim();
            if (string.IsNullOrEmpty(textToSend) || textToSend == SendDataPlaceholder) return;
            if (textToSend.Length % 2 != 0 || !Regex.IsMatch(textToSend, @"\A\b[0-9a-fA-F]+\b\Z"))
            {
                MessageBox.Show("Wprowadzony tekst musi być prawidłowym ciągiem heksadecymalnym (0-9, A-F) o parzystej długości.", "Błąd Walidacji", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            SendData(textToSend.ToUpper());
            txtSendData.Clear(); // Czyści pole, placeholder pojawi się przy utracie fokusu
        }

        private void TxtSendData_KeyDown(object? sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                e.SuppressKeyPress = true;
                btnManualSend.PerformClick();
            }
        }

        private void btnSetDistance_Click(object sender, EventArgs e)
        {
            string distanceText = txtDistance.Text.Trim();
            if (string.IsNullOrEmpty(distanceText) || distanceText == DistancePlaceholder) return;
            if (!int.TryParse(distanceText, out int distance) || distance < 0 || distance > 64)
            {
                MessageBox.Show("Wprowadź liczbę całkowitą z zakresu 0-64.", "Błąd Walidacji", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            string distanceHex = distance.ToString("X2");
            string repeatedDistance = string.Concat(Enumerable.Repeat(distanceHex, 4));
            string command = $"5533AACC{repeatedDistance}5533FFCC";
            SendData(command);
        }

        private void TxtDistance_KeyDown(object? sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                e.SuppressKeyPress = true;
                btnSetDistance.PerformClick();
            }
        }

        private void btnSaveLog_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.Filter = "Pliki tekstowe (*.txt)|*.txt|Wszystkie pliki (*.*)|*.*";
                saveFileDialog.FileName = $"log_{DateTime.Now:yyyy-MM-dd_HH-mm-ss}.txt";
                saveFileDialog.Title = "Zapisz log do pliku";
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        File.WriteAllText(saveFileDialog.FileName, rtbRawLog.Text);
                        MessageBox.Show("Log został pomyślnie zapisany.", "Sukces", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"Wystąpił błąd podczas zapisu pliku: {ex.Message}", "Błąd zapisu", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        private void btnClearLogs_Click(object sender, EventArgs e)
        {
            rtbRawLog.Clear();
            rtbLog00.Clear();
            rtbLog1.Clear();
            rtbLog2.Clear();
            rtbLog3.Clear();
            rtbLog4.Clear();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                _serialPort.Close();
            }
        }
    }
}