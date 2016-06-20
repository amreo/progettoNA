using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MySql.Data.MySqlClient;


namespace SettingsForm
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                //string di connessione
                string MyConnection2 = "datasource=localhost; port = 3306; username= arduino; password = arduino4you";

                //query di update
                string Query = "UPDATE dati_produzione.settings SET username='" + this.UserBox.Text + "', password = '" + this.PasswordBox.Text + "',servertport = '" + this.ServerPortBox.Text + "', serverip = '" + this.ServerIpBox.Text + "',maxretry = '" + this.RetryBox.Text + "',mac = '" + this.MacBox.Text + "',localip = '" + this.LocalIpBox.Text + "',lineaproduzione = '" + this.LineaProduzioneBox.Text + "',barcodetimeout = '" + this.TimeoutBox.Text + "';";

                //Connessione a MySQL
                MySqlConnection MyConn2 = new MySqlConnection(MyConnection2);

                MySqlCommand MyCommand2 = new MySqlCommand(Query, MyConn2);

                MySqlDataReader MyReader2;

                MyConn2.Open();

                MyReader2 = MyCommand2.ExecuteReader();

                MessageBox.Show("Data Updated");

                while (MyReader2.Read())
                {


                }

                MyConn2.Close(); //connessione chiusa
            }

            catch (Exception ex)
            {

                MessageBox.Show(ex.Message);
            }
            }
        }
    }
}
