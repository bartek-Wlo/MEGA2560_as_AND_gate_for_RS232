namespace SerialTerminalApp
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
                if (_serialPort != null)
                {
                    _serialPort.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tableLayoutPanelMain = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanelTop = new System.Windows.Forms.TableLayoutPanel();
            this.panelComPort = new System.Windows.Forms.Panel();
            this.cmbComPorts = new System.Windows.Forms.ComboBox();
            this.lblComPort = new System.Windows.Forms.Label();
            this.btnSnifMode = new System.Windows.Forms.Button();
            this.btnAndGateMode = new System.Windows.Forms.Button();
            this.btnToggleSending = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnSaveLog = new System.Windows.Forms.Button();
            this.txtSendData = new System.Windows.Forms.TextBox();
            this.btnManualSend = new System.Windows.Forms.Button();
            this.btnRefreshPorts = new System.Windows.Forms.Button();
            this.btnClearLogs = new System.Windows.Forms.Button();
            this.txtDistance = new System.Windows.Forms.TextBox();
            this.btnSetDistance = new System.Windows.Forms.Button();
            this.tableLayoutPanelBottom = new System.Windows.Forms.TableLayoutPanel();
            this.rtbRawLog = new System.Windows.Forms.RichTextBox();
            this.rtbLog00 = new System.Windows.Forms.RichTextBox();
            this.rtbLog1 = new System.Windows.Forms.RichTextBox();
            this.rtbLog2 = new System.Windows.Forms.RichTextBox();
            this.rtbLog3 = new System.Windows.Forms.RichTextBox();
            this.rtbLog4 = new System.Windows.Forms.RichTextBox();
            this.tableLayoutPanelMain.SuspendLayout();
            this.tableLayoutPanelTop.SuspendLayout();
            this.panelComPort.SuspendLayout();
            this.tableLayoutPanelBottom.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanelMain
            // 
            this.tableLayoutPanelMain.ColumnCount = 1;
            this.tableLayoutPanelMain.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelMain.Controls.Add(this.tableLayoutPanelTop, 0, 0);
            this.tableLayoutPanelMain.Controls.Add(this.tableLayoutPanelBottom, 0, 1);
            this.tableLayoutPanelMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelMain.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanelMain.Name = "tableLayoutPanelMain";
            this.tableLayoutPanelMain.RowCount = 2;
            this.tableLayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 140F));
            this.tableLayoutPanelMain.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanelMain.Size = new System.Drawing.Size(884, 561);
            this.tableLayoutPanelMain.TabIndex = 0;
            // 
            // tableLayoutPanelTop
            // 
            this.tableLayoutPanelTop.ColumnCount = 3;
            this.tableLayoutPanelTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanelTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanelTop.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanelTop.Controls.Add(this.panelComPort, 0, 0);
            this.tableLayoutPanelTop.Controls.Add(this.btnSnifMode, 0, 1);
            this.tableLayoutPanelTop.Controls.Add(this.btnAndGateMode, 0, 2);
            this.tableLayoutPanelTop.Controls.Add(this.btnToggleSending, 0, 3);
            this.tableLayoutPanelTop.Controls.Add(this.btnConnect, 1, 0);
            this.tableLayoutPanelTop.Controls.Add(this.btnSaveLog, 1, 1);
            this.tableLayoutPanelTop.Controls.Add(this.txtSendData, 1, 2);
            this.tableLayoutPanelTop.Controls.Add(this.btnManualSend, 1, 3);
            this.tableLayoutPanelTop.Controls.Add(this.btnRefreshPorts, 2, 0);
            this.tableLayoutPanelTop.Controls.Add(this.btnClearLogs, 2, 1);
            this.tableLayoutPanelTop.Controls.Add(this.txtDistance, 2, 2);
            this.tableLayoutPanelTop.Controls.Add(this.btnSetDistance, 2, 3);
            this.tableLayoutPanelTop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelTop.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanelTop.Name = "tableLayoutPanelTop";
            this.tableLayoutPanelTop.RowCount = 4;
            this.tableLayoutPanelTop.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanelTop.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanelTop.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanelTop.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanelTop.Size = new System.Drawing.Size(878, 134);
            this.tableLayoutPanelTop.TabIndex = 0;
            // 
            // panelComPort
            // 
            this.panelComPort.Controls.Add(this.cmbComPorts);
            this.panelComPort.Controls.Add(this.lblComPort);
            this.panelComPort.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelComPort.Location = new System.Drawing.Point(3, 3);
            this.panelComPort.Name = "panelComPort";
            this.panelComPort.Size = new System.Drawing.Size(286, 27);
            this.panelComPort.TabIndex = 0;
            // 
            // cmbComPorts
            // 
            this.cmbComPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbComPorts.FormattingEnabled = true;
            this.cmbComPorts.Location = new System.Drawing.Point(85, 3);
            this.cmbComPorts.Name = "cmbComPorts";
            this.cmbComPorts.Size = new System.Drawing.Size(121, 21);
            this.cmbComPorts.TabIndex = 1;
            // 
            // lblComPort
            // 
            this.lblComPort.AutoSize = true;
            this.lblComPort.Location = new System.Drawing.Point(4, 6);
            this.lblComPort.Name = "lblComPort";
            this.lblComPort.Size = new System.Drawing.Size(75, 13);
            this.lblComPort.TabIndex = 0;
            this.lblComPort.Text = "Wybierz Port:";
            // 
            // btnSnifMode
            // 
            this.btnSnifMode.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnSnifMode.Location = new System.Drawing.Point(3, 36);
            this.btnSnifMode.Name = "btnSnifMode";
            this.btnSnifMode.Size = new System.Drawing.Size(286, 27);
            this.btnSnifMode.TabIndex = 1;
            this.btnSnifMode.Text = "Tryb pracy SNIFer";
            this.btnSnifMode.UseVisualStyleBackColor = true;
            this.btnSnifMode.Click += new System.EventHandler(this.btnSnifMode_Click);
            // 
            // btnAndGateMode
            // 
            this.btnAndGateMode.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAndGateMode.Location = new System.Drawing.Point(3, 69);
            this.btnAndGateMode.Name = "btnAndGateMode";
            this.btnAndGateMode.Size = new System.Drawing.Size(286, 27);
            this.btnAndGateMode.TabIndex = 2;
            this.btnAndGateMode.Text = "Tryb pracy AND gate";
            this.btnAndGateMode.UseVisualStyleBackColor = true;
            this.btnAndGateMode.Click += new System.EventHandler(this.btnAndGateMode_Click);
            // 
            // btnToggleSending
            // 
            this.btnToggleSending.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnToggleSending.Location = new System.Drawing.Point(3, 102);
            this.btnToggleSending.Name = "btnToggleSending";
            this.btnToggleSending.Size = new System.Drawing.Size(286, 29);
            this.btnToggleSending.TabIndex = 3;
            this.btnToggleSending.Text = "Włącz/Wyłącz wysyłanie";
            this.btnToggleSending.UseVisualStyleBackColor = true;
            this.btnToggleSending.Click += new System.EventHandler(this.btnToggleSending_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnConnect.Location = new System.Drawing.Point(295, 3);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(286, 27);
            this.btnConnect.TabIndex = 4;
            this.btnConnect.Text = "Połącz";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnSaveLog
            // 
            this.btnSaveLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnSaveLog.Location = new System.Drawing.Point(295, 36);
            this.btnSaveLog.Name = "btnSaveLog";
            this.btnSaveLog.Size = new System.Drawing.Size(286, 27);
            this.btnSaveLog.TabIndex = 5;
            this.btnSaveLog.Text = "Zapisz dane do pliku .txt";
            this.btnSaveLog.UseVisualStyleBackColor = true;
            this.btnSaveLog.Click += new System.EventHandler(this.btnSaveLog_Click);
            // 
            // txtSendData
            // 
            this.txtSendData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtSendData.Location = new System.Drawing.Point(295, 69);
            this.txtSendData.Name = "txtSendData";
            this.txtSendData.Size = new System.Drawing.Size(286, 20);
            this.txtSendData.TabIndex = 6;
            // 
            // btnManualSend
            // 
            this.btnManualSend.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnManualSend.Location = new System.Drawing.Point(295, 102);
            this.btnManualSend.Name = "btnManualSend";
            this.btnManualSend.Size = new System.Drawing.Size(286, 29);
            this.btnManualSend.TabIndex = 7;
            this.btnManualSend.Text = "Wyślij dane";
            this.btnManualSend.UseVisualStyleBackColor = true;
            this.btnManualSend.Click += new System.EventHandler(this.btnManualSend_Click);
            // 
            // btnRefreshPorts
            // 
            this.btnRefreshPorts.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnRefreshPorts.Location = new System.Drawing.Point(587, 3);
            this.btnRefreshPorts.Name = "btnRefreshPorts";
            this.btnRefreshPorts.Size = new System.Drawing.Size(288, 27);
            this.btnRefreshPorts.TabIndex = 8;
            this.btnRefreshPorts.Text = "Odśwież";
            this.btnRefreshPorts.UseVisualStyleBackColor = true;
            this.btnRefreshPorts.Click += new System.EventHandler(this.btnRefreshPorts_Click);
            // 
            // btnClearLogs
            // 
            this.btnClearLogs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClearLogs.Location = new System.Drawing.Point(587, 36);
            this.btnClearLogs.Name = "btnClearLogs";
            this.btnClearLogs.Size = new System.Drawing.Size(288, 27);
            this.btnClearLogs.TabIndex = 9;
            this.btnClearLogs.Text = "Wyczyść dane";
            this.btnClearLogs.UseVisualStyleBackColor = true;
            this.btnClearLogs.Click += new System.EventHandler(this.btnClearLogs_Click);
            // 
            // txtDistance
            // 
            this.txtDistance.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtDistance.Location = new System.Drawing.Point(587, 69);
            this.txtDistance.Name = "txtDistance";
            this.txtDistance.Size = new System.Drawing.Size(288, 20);
            this.txtDistance.TabIndex = 10;
            // 
            // btnSetDistance
            // 
            this.btnSetDistance.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnSetDistance.Location = new System.Drawing.Point(587, 102);
            this.btnSetDistance.Name = "btnSetDistance";
            this.btnSetDistance.Size = new System.Drawing.Size(288, 29);
            this.btnSetDistance.TabIndex = 11;
            this.btnSetDistance.Text = "Ustaw odległość";
            this.btnSetDistance.UseVisualStyleBackColor = true;
            this.btnSetDistance.Click += new System.EventHandler(this.btnSetDistance_Click);
            // 
            // tableLayoutPanelBottom
            // 
            this.tableLayoutPanelBottom.ColumnCount = 2;
            this.tableLayoutPanelBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelBottom.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanelBottom.Controls.Add(this.rtbRawLog, 0, 0);
            this.tableLayoutPanelBottom.Controls.Add(this.rtbLog00, 1, 0);
            this.tableLayoutPanelBottom.Controls.Add(this.rtbLog1, 0, 1);
            this.tableLayoutPanelBottom.Controls.Add(this.rtbLog2, 1, 1);
            this.tableLayoutPanelBottom.Controls.Add(this.rtbLog3, 0, 2);
            this.tableLayoutPanelBottom.Controls.Add(this.rtbLog4, 1, 2);
            this.tableLayoutPanelBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanelBottom.Location = new System.Drawing.Point(3, 143);
            this.tableLayoutPanelBottom.Name = "tableLayoutPanelBottom";
            this.tableLayoutPanelBottom.RowCount = 3;
            this.tableLayoutPanelBottom.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanelBottom.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanelBottom.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanelBottom.Size = new System.Drawing.Size(878, 415);
            this.tableLayoutPanelBottom.TabIndex = 1;
            // 
            // rtbRawLog
            // 
            this.rtbRawLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtbRawLog.Location = new System.Drawing.Point(3, 3);
            this.rtbRawLog.Name = "rtbRawLog";
            this.rtbRawLog.ReadOnly = true;
            this.rtbRawLog.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.rtbRawLog.Size = new System.Drawing.Size(433, 132);
            this.rtbRawLog.TabIndex = 0;
            this.rtbRawLog.Text = "";
            // 
            // rtbLog00
            // 
            this.rtbLog00.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtbLog00.Location = new System.Drawing.Point(442, 3);
            this.rtbLog00.Name = "rtbLog00";
            this.rtbLog00.ReadOnly = true;
            this.rtbLog00.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.rtbLog00.Size = new System.Drawing.Size(433, 132);
            this.rtbLog00.TabIndex = 1;
            this.rtbLog00.Text = "";
            // 
            // rtbLog1
            // 
            this.rtbLog1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtbLog1.Location = new System.Drawing.Point(3, 141);
            this.rtbLog1.Name = "rtbLog1";
            this.rtbLog1.ReadOnly = true;
            this.rtbLog1.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.rtbLog1.Size = new System.Drawing.Size(433, 132);
            this.rtbLog1.TabIndex = 2;
            this.rtbLog1.Text = "";
            // 
            // rtbLog2
            // 
            this.rtbLog2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtbLog2.Location = new System.Drawing.Point(442, 141);
            this.rtbLog2.Name = "rtbLog2";
            this.rtbLog2.ReadOnly = true;
            this.rtbLog2.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.rtbLog2.Size = new System.Drawing.Size(433, 132);
            this.rtbLog2.TabIndex = 3;
            this.rtbLog2.Text = "";
            // 
            // rtbLog3
            // 
            this.rtbLog3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtbLog3.Location = new System.Drawing.Point(3, 279);
            this.rtbLog3.Name = "rtbLog3";
            this.rtbLog3.ReadOnly = true;
            this.rtbLog3.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.rtbLog3.Size = new System.Drawing.Size(433, 133);
            this.rtbLog3.TabIndex = 4;
            this.rtbLog3.Text = "";
            // 
            // rtbLog4
            // 
            this.rtbLog4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtbLog4.Location = new System.Drawing.Point(442, 279);
            this.rtbLog4.Name = "rtbLog4";
            this.rtbLog4.ReadOnly = true;
            this.rtbLog4.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.ForcedVertical;
            this.rtbLog4.Size = new System.Drawing.Size(433, 133);
            this.rtbLog4.TabIndex = 5;
            this.rtbLog4.Text = "";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(884, 561);
            this.Controls.Add(this.tableLayoutPanelMain);
            this.MinimumSize = new System.Drawing.Size(600, 400);
            this.Name = "Form1";
            this.Text = "Terminal RS232 - do Serial0 MEGA2560 - do sterowania przełączania trybu pracy";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.tableLayoutPanelMain.ResumeLayout(false);
            this.tableLayoutPanelTop.ResumeLayout(false);
            this.tableLayoutPanelTop.PerformLayout();
            this.panelComPort.ResumeLayout(false);
            this.panelComPort.PerformLayout();
            this.tableLayoutPanelBottom.ResumeLayout(false);
            this.tableLayoutPanelBottom.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelMain;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelTop;
        private System.Windows.Forms.Panel panelComPort;
        private System.Windows.Forms.ComboBox cmbComPorts;
        private System.Windows.Forms.Label lblComPort;
        private System.Windows.Forms.Button btnSnifMode;
        private System.Windows.Forms.Button btnAndGateMode;
        private System.Windows.Forms.Button btnToggleSending;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnSaveLog;
        private System.Windows.Forms.TextBox txtSendData;
        private System.Windows.Forms.Button btnManualSend;
        private System.Windows.Forms.Button btnRefreshPorts;
        private System.Windows.Forms.Button btnClearLogs;
        private System.Windows.Forms.TextBox txtDistance;
        private System.Windows.Forms.Button btnSetDistance;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanelBottom;
        private System.Windows.Forms.RichTextBox rtbRawLog;
        private System.Windows.Forms.RichTextBox rtbLog00;
        private System.Windows.Forms.RichTextBox rtbLog1;
        private System.Windows.Forms.RichTextBox rtbLog2;
        private System.Windows.Forms.RichTextBox rtbLog3;
        private System.Windows.Forms.RichTextBox rtbLog4;
    }
}