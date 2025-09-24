namespace praktykaZawdowa;
using System;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.ComponentModel;
using System.Globalization;

public partial class Form1 : Form
{
    // --- DEKLARACJA KOMPONENTÓW INTERFEJSU UŻYTKOWNIKA ---
    private Button? btnSnifMode;
    private Button? btnAndGateMode;
    private Button? btnToggleSending;
    private Button? btnManualSend;
    private Button? btnConnect;
    private Button? btnRefreshPorts;
    private TextBox? txtSendData;
    private ComboBox? cmbComPorts;
    private Label? lblComPort;
    private Label? lblSendData;

    private TextBox? txtMainLog;
    private TextBox? txtLog1;
    private TextBox? txtLog2;
    private TextBox? txtLog3;
    private TextBox? txtLog4;
    private TableLayoutPanel? tableLayoutPanelLogs;

    private SerialPort? serialPort;
    private IContainer? components = null;

    public Form1()
    {
        InitializeComponent();
        InitializeSerialPort();
        LoadAvailableComPorts();
    }

    #region Inicjalizacja GUI

    private void InitializeComponent()
    {
        this.components = new Container();
        this.btnSnifMode = new Button();
        this.btnAndGateMode = new Button();
        this.btnToggleSending = new Button();
        this.btnManualSend = new Button();
        this.btnConnect = new Button();
        this.btnRefreshPorts = new Button();
        this.txtSendData = new TextBox();
        this.cmbComPorts = new ComboBox();
        this.lblComPort = new Label();
        this.lblSendData = new Label();
        this.txtMainLog = new TextBox();
        this.tableLayoutPanelLogs = new TableLayoutPanel();
        this.txtLog1 = new TextBox();
        this.txtLog2 = new TextBox();
        this.txtLog3 = new TextBox();
        this.txtLog4 = new TextBox();
        
        this.SuspendLayout();
        
        this.ClientSize = new System.Drawing.Size(800, 600);
        this.Text = "Terminal RS232 - do Serial0 MEGA2560 - do sterowania przełączania trybu pracy";
        this.Name = "Form1";
        this.StartPosition = FormStartPosition.CenterScreen;
        this.WindowState = FormWindowState.Maximized;
        this.FormClosing += new FormClosingEventHandler(this.Form1_FormClosing);

        this.lblComPort.AutoSize = true;
        this.lblComPort.Location = new System.Drawing.Point(12, 15);
        this.lblComPort.Text = "Wybierz Port COM:";

        this.cmbComPorts.DropDownStyle = ComboBoxStyle.DropDownList;
        this.cmbComPorts.FormattingEnabled = true;
        this.cmbComPorts.Location = new System.Drawing.Point(120, 12);
        this.cmbComPorts.Size = new System.Drawing.Size(121, 21);

        this.btnConnect.Location = new System.Drawing.Point(250, 10);
        this.btnConnect.Size = new System.Drawing.Size(100, 23);
        this.btnConnect.Text = "Połącz";
        this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);

        this.btnRefreshPorts.Location = new System.Drawing.Point(360, 10);
        this.btnRefreshPorts.Size = new System.Drawing.Size(100, 23);
        this.btnRefreshPorts.Text = "Odśwież";
        this.btnRefreshPorts.Click += new System.EventHandler(this.btnRefreshPorts_Click);

        this.btnSnifMode.Location = new System.Drawing.Point(15, 60);
        this.btnSnifMode.Size = new System.Drawing.Size(226, 30);
        this.btnSnifMode.Text = "Tryb pracy SNIFer";
        this.btnSnifMode.Click += new System.EventHandler(this.btnSnifMode_Click);

        this.btnAndGateMode.Location = new System.Drawing.Point(15, 100);
        this.btnAndGateMode.Size = new System.Drawing.Size(226, 30);
        this.btnAndGateMode.Text = "Tryb pracy AND gate";
        this.btnAndGateMode.Click += new System.EventHandler(this.btnAndGateMode_Click);

        this.btnToggleSending.Location = new System.Drawing.Point(15, 140);
        this.btnToggleSending.Size = new System.Drawing.Size(226, 30);
        this.btnToggleSending.Text = "Włącz/Wyłącz wysyłanie danych";
        this.btnToggleSending.Click += new System.EventHandler(this.btnToggleSending_Click);

        this.lblSendData.AutoSize = true;
        this.lblSendData.Location = new System.Drawing.Point(260, 60);
        this.lblSendData.Text = "Dane do wysłania (HEX, np. 123456 jako 0x12 0x34 0x56):";

        this.txtSendData.Location = new System.Drawing.Point(263, 76);
        this.txtSendData.Size = new System.Drawing.Size(300, 20);
        this.txtSendData.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);
        // ZMIANA: Dodanie obsługi zdarzenia naciśnięcia klawisza
        this.txtSendData.KeyDown += new KeyEventHandler(this.txtSendData_KeyDown);

        this.btnManualSend.Location = new System.Drawing.Point(263, 102);
        this.btnManualSend.Size = new System.Drawing.Size(100, 23);
        this.btnManualSend.Text = "Wyślij";
        this.btnManualSend.Click += new System.EventHandler(this.btnManualSend_Click);

        this.txtMainLog.Location = new System.Drawing.Point(15, 190);
        this.txtMainLog.Multiline = true;
        this.txtMainLog.ReadOnly = true;
        this.txtMainLog.ScrollBars = ScrollBars.Vertical;
        this.txtMainLog.Size = new System.Drawing.Size(760, 120);
        this.txtMainLog.Font = new Font("Consolas", 9.75F, FontStyle.Regular, GraphicsUnit.Point);
        this.txtMainLog.Anchor = (AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right);

        this.tableLayoutPanelLogs.Location = new System.Drawing.Point(15, 320);
        this.tableLayoutPanelLogs.Size = new System.Drawing.Size(760, 260);
        this.tableLayoutPanelLogs.Anchor = (AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right);
        this.tableLayoutPanelLogs.ColumnCount = 2;
        this.tableLayoutPanelLogs.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
        this.tableLayoutPanelLogs.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
        this.tableLayoutPanelLogs.RowCount = 2;
        this.tableLayoutPanelLogs.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));
        this.tableLayoutPanelLogs.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));

        var logTextBoxes = new[] { this.txtLog1, this.txtLog2, this.txtLog3, this.txtLog4 };
        foreach (var textBox in logTextBoxes)
        {
            textBox.Multiline = true;
            textBox.ReadOnly = true;
            textBox.ScrollBars = ScrollBars.Vertical;
            textBox.Font = new Font("Consolas", 9.75F, FontStyle.Regular, GraphicsUnit.Point);
            textBox.Dock = DockStyle.Fill;
        }

        this.tableLayoutPanelLogs.Controls.Add(this.txtLog1, 0, 0);
        this.tableLayoutPanelLogs.Controls.Add(this.txtLog2, 1, 0);
        this.tableLayoutPanelLogs.Controls.Add(this.txtLog3, 0, 1);
        this.tableLayoutPanelLogs.Controls.Add(this.txtLog4, 1, 1);
        
        this.Controls.AddRange(new Control[] {
            this.lblComPort, this.cmbComPorts, this.btnConnect, this.btnRefreshPorts,
            this.btnSnifMode, this.btnAndGateMode, this.btnToggleSending, this.lblSendData,
            this.txtSendData, this.btnManualSend, this.txtMainLog, this.tableLayoutPanelLogs
        });
        
        this.ResumeLayout(false);
        this.PerformLayout();
        ToggleControls(false);
    }

    #endregion

    #region Logika Aplikacji

    private void InitializeSerialPort()
    {
        serialPort = new SerialPort();
        serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceived);
    }

    private void LoadAvailableComPorts()
    {
        if (cmbComPorts == null) return;
        cmbComPorts.Items.Clear();
        string[] portNames = SerialPort.GetPortNames();
        if (portNames.Length > 0)
        {
            cmbComPorts.Items.AddRange(portNames);
            cmbComPorts.SelectedIndex = 0;
        }
        else
        {
            cmbComPorts.Text = "Brak portów";
        }
    }

    private void btnConnect_Click(object? sender, EventArgs e)
    {
        if (serialPort == null) return;
        if (serialPort.IsOpen)
        {
            serialPort.Close();
            if (btnConnect != null) btnConnect.Text = "Połącz";
            ToggleControls(false);
        }
        else
        {
            if (cmbComPorts?.SelectedItem == null)
            {
                MessageBox.Show("Proszę wybrać port COM.", "Informacja", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            try
            {
                serialPort.PortName = cmbComPorts.SelectedItem.ToString() ?? "";
                serialPort.BaudRate = 115200;
                serialPort.Parity = Parity.None;
                serialPort.DataBits = 8;
                serialPort.StopBits = StopBits.One;
                serialPort.Open();
                if (btnConnect != null) btnConnect.Text = "Rozłącz";
                ToggleControls(true);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Błąd otwarcia portu COM: " + ex.Message, "Błąd", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }

    private void btnRefreshPorts_Click(object? sender, EventArgs e) => LoadAvailableComPorts();

    private void SendData(byte[] data)
    {
        if (serialPort != null && serialPort.IsOpen)
        {
            try
            {
                serialPort.Write(data, 0, data.Length);
                string hexString = BitConverter.ToString(data).Replace("-", " ");
                LogMessage("WYSŁANO (HEX): " + hexString, txtMainLog);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Błąd podczas wysyłania danych: " + ex.Message, "Błąd", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        else
        {
            MessageBox.Show("Port COM nie jest otwarty.", "Informacja", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
    
    // ZMIANA: Logika prefiksów przeniesiona tutaj
    private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
    {
        if (serialPort == null || !serialPort.IsOpen) return;
        try
        {
            int bytesToRead = serialPort.BytesToRead;
            byte[] buffer = new byte[bytesToRead];
            serialPort.Read(buffer, 0, bytesToRead);

            string receivedText = Encoding.ASCII.GetString(buffer);
            string messageContent = receivedText.Trim();

            TextBox? targetLog;
            string logPrefix;

            if (receivedText.Contains("01:"))
            {
                targetLog = txtLog1;
                logPrefix = ""; // Brak prefiksu dla dedykowanego okna
            }
            else if (receivedText.Contains("02:"))
            {
                targetLog = txtLog2;
                logPrefix = "";
            }
            else if (receivedText.Contains("03:"))
            {
                targetLog = txtLog3;
                logPrefix = "";
            }
            else if (receivedText.Contains("04:"))
            {
                targetLog = txtLog4;
                logPrefix = "";
            }
            else
            {
                targetLog = txtMainLog;
                logPrefix = "OTRZYMANO: "; // Prefiks tylko dla głównego okna
            }

            LogMessage(logPrefix + messageContent, targetLog);
        }
        catch (Exception)
        {
            // Pusty blok catch
        }
    }

    private void btnSnifMode_Click(object? sender, EventArgs e) => SendData(new byte[] { 0x55, 0x33, 0x0F, 0xCC });
    private void btnAndGateMode_Click(object? sender, EventArgs e) => SendData(new byte[] { 0x55, 0x33, 0xF0, 0xCC });
    private void btnToggleSending_Click(object? sender, EventArgs e) => SendData(new byte[] { 0x55, 0x33, 0xEE, 0xCC });

    private void btnManualSend_Click(object? sender, EventArgs e)
    {
        if (txtSendData == null) return;
        string hexInput = txtSendData.Text.Replace(" ", "").Trim();
        if (string.IsNullOrEmpty(hexInput))
        {
            MessageBox.Show("Pole danych do wysłania jest puste.", "Informacja", MessageBoxButtons.OK, MessageBoxIcon.Information);
            return;
        }

        if (hexInput.Length % 2 != 0)
        {
            MessageBox.Show("Nieprawidłowa długość ciągu HEX. Liczba znaków musi być parzysta.", "Błąd formatu", MessageBoxButtons.OK, MessageBoxIcon.Error);
            return;
        }

        try
        {
            byte[] data = new byte[hexInput.Length / 2];
            for (int i = 0; i < data.Length; i++)
            {
                string hexByte = hexInput.Substring(i * 2, 2);
                data[i] = byte.Parse(hexByte, NumberStyles.HexNumber);
            }
            SendData(data);
        }
        catch (Exception ex)
        {
            MessageBox.Show("Nieprawidłowy format danych HEX. Wpisz ciąg znaków 0-9 i A-F.\nSzczegóły: " + ex.Message, "Błąd formatu", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
    
    // ZMIANA: Nowa metoda do obsługi naciśnięcia klawisza Enter
    private void txtSendData_KeyDown(object? sender, KeyEventArgs e)
    {
        if (e.KeyCode == Keys.Enter)
        {
            // Wywołaj zdarzenie kliknięcia przycisku wysyłania
            btnManualSend_Click(sender, e);
            // Zablokuj domyślny dźwięk systemowy dla klawisza Enter
            e.SuppressKeyPress = true; 
        }
    }

    private void Form1_FormClosing(object? sender, FormClosingEventArgs e)
    {
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
        }
    }

    private string GetTimestamp() => DateTime.Now.ToString("HH:mm:ss.fff") + ": ";
    
    private void LogMessage(string message, TextBox? targetTextBox)
    {
        if (targetTextBox == null) return;
        string timestampedMessage = GetTimestamp() + message + Environment.NewLine;
        if (targetTextBox.InvokeRequired)
        {
            targetTextBox.Invoke(new Action(() => targetTextBox.AppendText(timestampedMessage)));
        }
        else
        {
            targetTextBox.AppendText(timestampedMessage);
        }
    }
    
    private void ToggleControls(bool isEnabled)
    {
        var controlsToToggle = new Control?[] { 
            btnSnifMode, btnAndGateMode, btnToggleSending, 
            btnManualSend, txtSendData 
        };
        foreach (var control in controlsToToggle)
        {
            if (control != null) control.Enabled = isEnabled;
        }
        if (cmbComPorts != null) cmbComPorts.Enabled = !isEnabled;
        if (btnRefreshPorts != null) btnRefreshPorts.Enabled = !isEnabled;
    }

    #endregion

    #region Czyszczenie zasobów

    protected override void Dispose(bool disposing)
    {
        if (disposing && (components != null))
        {
            components.Dispose();
        }
        base.Dispose(disposing);
    }

    #endregion
}