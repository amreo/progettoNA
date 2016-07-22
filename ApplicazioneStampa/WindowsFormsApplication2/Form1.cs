using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows.Forms;
using System.Data.SQLite;

namespace WindowsFormsApplication2
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        string strConn = "Data Source = MyDatabase;";
        SQLiteConnection m_dbConnection;
        SQLiteCommand command;
        
        private void button1_Click(object sender, EventArgs e)
        {
            


            if (comboBox1.Text == "")
            {
                MessageBox.Show("Selezionare un prodotto.", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else
            {
                if (numericUpDown1.Text == "0")
                {
                    MessageBox.Show("Selezionare il numero di copie.", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
                else
                {
                    string message = "Stai per stampare " + numericUpDown1.Text + " copie del prodotto: " + comboBox1.Text + "." + "\n" + "Continuare?";
                    string caption = "Conferma di stampa";
                    MessageBoxButtons buttons = MessageBoxButtons.YesNo;
                    DialogResult result;
                    result = MessageBox.Show(message, caption, buttons);
                    if (result == System.Windows.Forms.DialogResult.Yes)
                    {
                        m_dbConnection = new SQLiteConnection(strConn);
                        string sql = "SELECT Barcode FROM Prodotti WHERE Nome = '" + comboBox1.Text + "';";
                        command = new SQLiteCommand(sql, m_dbConnection);
                        string search ="";
                        m_dbConnection.Open();
                        SQLiteDataReader Ricerca = command.ExecuteReader();
                        while (Ricerca.Read())
                        {
                            search = search + Ricerca["Barcode"];
                            
                        }
                        Ricerca.Close();
                        m_dbConnection.Close();
                        byte[] stampa = new byte[search.Length];

                        for (int i = 0; i < search.Length; i++)
                           
                        {
                            stampa[i] = (byte)search[i];
                        }

                        byte[] spazio = { 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A };
                        byte[] code39 = { 0x1D, 0x6B, 0x04 };
                        byte[] fine = { 0x00 };
                        byte[] taglio = { 0x1C, 0xC0, 0x34 };
                        
                        serialPort1.PortName = "COM40";
                        serialPort1.BaudRate = 115200;
                        serialPort1.DataBits = 8;
                        serialPort1.Parity = Parity.None;
                        serialPort1.StopBits = StopBits.One;

                        for (int i = 0; i < numericUpDown1.Value; i++)
                        {
                            serialPort1.Open();

                            serialPort1.Write(spazio, 0, spazio.Length);
                            serialPort1.Write(code39, 0, code39.Length);
                            serialPort1.Write(stampa, 0, stampa.Length);
                            serialPort1.Write(fine, 0, fine.Length);
                            serialPort1.Write(taglio, 0, taglio.Length);

                            serialPort1.Close();
                        }
                    }
                }
            }
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "")
            {
                MessageBox.Show("Inserire prodotto.", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else
            {
                if (textBox2.Text == "")
                {
                    MessageBox.Show("Inserire un barcode.", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
                else
                {
                    m_dbConnection = new SQLiteConnection(strConn);
                    string sql = "insert into Prodotti (Nome, Barcode) values ('" + textBox1.Text + "', " + textBox2.Text + ")";

                    m_dbConnection.Open();

                    command = new SQLiteCommand(sql, m_dbConnection);
                    command.ExecuteNonQuery();
                    string sql2 = "SELECT Nome FROM Prodotti;";
                    command = new SQLiteCommand(sql2, m_dbConnection);
                    SQLiteDataReader Elementi = command.ExecuteReader();
                    comboBox1.Items.Clear();
                    comboBox2.Items.Clear();
                    comboBox3.Items.Clear();
                    comboBox4.Items.Clear();
                    while (Elementi.Read())
                    {
                        comboBox1.Items.Add(Elementi["Nome"]);
                        comboBox2.Items.Add(Elementi["Nome"]);
                        comboBox3.Items.Add(Elementi["Nome"]);
                        comboBox4.Items.Add(Elementi["Nome"]);
                    }
                    Elementi.Close();

                    m_dbConnection.Close();
                    MessageBox.Show("Prodotto inserito.", "Avviso", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    textBox1.Text = "";
                    textBox2.Text = "";
                }
            }
        }



        private void button4_Click(object sender, EventArgs e)
        {
            if (comboBox3.Text == "")
            {
                MessageBox.Show("Selezionare un prodotto.", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else
            {
                string message = "Sicuro di voler eliminare definitivamente il prodotto " + comboBox3.Text + "?";
                string caption = "Attenzione";
                MessageBoxButtons buttons = MessageBoxButtons.YesNo;
                MessageBoxIcon icon = MessageBoxIcon.Warning;
                DialogResult result;
                result = MessageBox.Show(message, caption, buttons, icon);
                if (result == System.Windows.Forms.DialogResult.Yes)
                {
                    m_dbConnection = new SQLiteConnection(strConn);
                    string sql = "DELETE FROM Prodotti WHERE Nome IS '" + comboBox3.Text + "';";

                    m_dbConnection.Open();

                    command = new SQLiteCommand(sql, m_dbConnection);
                    command.ExecuteNonQuery();
                    string sql2 = "SELECT Nome FROM Prodotti;";
                    command = new SQLiteCommand(sql2, m_dbConnection);
                    SQLiteDataReader Elementi = command.ExecuteReader();
                    comboBox1.Items.Clear();
                    comboBox2.Items.Clear();
                    comboBox3.Items.Clear();
                    comboBox4.Items.Clear();
                    while (Elementi.Read())
                    {
                        comboBox1.Items.Add(Elementi["Nome"]);
                        comboBox2.Items.Add(Elementi["Nome"]);
                        comboBox3.Items.Add(Elementi["Nome"]);
                        comboBox4.Items.Add(Elementi["Nome"]);
                    }
                    Elementi.Close();

                    m_dbConnection.Close();
                    MessageBox.Show("Prodotto eliminato.", "Avviso", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    comboBox3.Text = "";
                }
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (comboBox2.Text == "")
            {
                MessageBox.Show("Selezionare un prodotto.", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else
            {
                if (textBox3.Text == "")
                {
                    MessageBox.Show("Inserire un barcode.", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
                else
                {

                    string message = "Sicuro di voler modificare questo prodotto in: \n" + comboBox2.Text + " con barcode " + textBox3.Text + " ?";
                    string caption = "Attenzione";
                    MessageBoxButtons buttons = MessageBoxButtons.YesNo;
                    MessageBoxIcon icon = MessageBoxIcon.Warning;
                    DialogResult result;
                    result = MessageBox.Show(message, caption, buttons, icon);
                    if (result == System.Windows.Forms.DialogResult.Yes)
                    {
                        m_dbConnection = new SQLiteConnection(strConn);
                        string sql = "UPDATE Prodotti SET Barcode = " + textBox3.Text + " WHERE Nome = '" + comboBox2.Text + "';";

                        m_dbConnection.Open();

                        command = new SQLiteCommand(sql, m_dbConnection);
                        command.ExecuteNonQuery();
                        string sql2 = "SELECT Nome FROM Prodotti;";
                        command = new SQLiteCommand(sql2, m_dbConnection);
                        SQLiteDataReader Elementi = command.ExecuteReader();
                        comboBox1.Items.Clear();
                        comboBox2.Items.Clear();
                        comboBox3.Items.Clear();
                        comboBox4.Items.Clear();
                        while (Elementi.Read())
                        {
                            comboBox1.Items.Add(Elementi["Nome"]);
                            comboBox2.Items.Add(Elementi["Nome"]);
                            comboBox3.Items.Add(Elementi["Nome"]);
                            comboBox4.Items.Add(Elementi["Nome"]);
                        }
                        Elementi.Close();

                        m_dbConnection.Close();
                        MessageBox.Show("Prodotto modificato.", "Avviso", MessageBoxButtons.OK, MessageBoxIcon.Information);
                        comboBox2.Text = "";
                        textBox3.Text = "";
                    }
                }
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (comboBox4.Text == "")
            {
                MessageBox.Show("Selezionare un prodotto.", "Errore", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            else
            {
                m_dbConnection = new SQLiteConnection(strConn);
                string sql = "SELECT Barcode FROM Prodotti WHERE Nome = '" + comboBox4.Text + "';";
                command = new SQLiteCommand(sql, m_dbConnection);
                string risultato = "Prodotto selezionato:" + "\n" + comboBox4.Text + "\t" + "Barcode: ";
                m_dbConnection.Open();
                SQLiteDataReader Ricerca = command.ExecuteReader();
                while (Ricerca.Read())
                {
                    risultato = risultato + Ricerca["Barcode"];
                }
                Ricerca.Close();
                m_dbConnection.Close();

                MessageBox.Show(risultato, "Ricerca", MessageBoxButtons.OK, MessageBoxIcon.Information);
                comboBox4.Text = "";
            }
        }

        private void Form1_Load_1(object sender, EventArgs e)
        {
            // Caricare nelle 4 combobox gli stessi elementi che vengono presi dal database dalla colonna Nome tabella Prodotti database MyDatabase;
            m_dbConnection = new SQLiteConnection(strConn);
            string sql = "SELECT Nome FROM Prodotti;";
            command = new SQLiteCommand(sql, m_dbConnection);

            m_dbConnection.Open();

            SQLiteDataReader Elementi = command.ExecuteReader();
            while (Elementi.Read())
            {
                comboBox1.Items.Add(Elementi["Nome"]);
                comboBox2.Items.Add(Elementi["Nome"]);
                comboBox3.Items.Add(Elementi["Nome"]);
                comboBox4.Items.Add(Elementi["Nome"]);
            }
            Elementi.Close();
            m_dbConnection.Close();

        }

       
    }
}
