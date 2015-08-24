using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WF___Tic_Tac_Toe
{
    public partial class Form1 : Form
    {
        private bool user_turn = true;
        private int turn_count = 0;
        private int user_score = 0;
        private int ai_score = 0;
        private int draws = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("By Lingurar Petru-Mugurel\n\n\nCheck sources on Github?",
                "About TTT", MessageBoxButtons.OKCancel)
                == System.Windows.Forms.DialogResult.OK)
            {
                System.Diagnostics.Process.Start("www.goagal.ro");
            }

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Do you really want to exit?", "Warning",
                MessageBoxButtons.OKCancel) == System.Windows.Forms.DialogResult.OK)
                Application.Exit();
        }

        private void button_click(object sender, EventArgs e)
        {
            Button b = (Button)sender;
            if (user_turn)
                b.Text = "X";
            else
                b.Text = "O";

            // change the turn
            user_turn = !user_turn;

            // prevent the user to change the recorded state
            b.Enabled = false;

            // for every move increase the turn count
            ++turn_count;

            // after any player could have made 3 moved check for a winner
            if (turn_count >= 5)
                winner_check();

        }

        private void winner_check()
        {
            bool winner = false;

            /**
             * The third check is to make sure all buttons on the line checked
             * contain X or 0. 
             * If one is disabled - was used, and all are the same,
             * all are disabled - all contain X or 0.
             */

            // horizontal lines
            if ((A1.Text == A2.Text) && (A1.Text == A3.Text) && (!A1.Enabled))
                winner = true;
            else if ((B1.Text == B2.Text) && (B1.Text == B3.Text) && (!B1.Enabled))
                winner = true;
            else if ((C1.Text == C2.Text) && (C1.Text == C3.Text) && (!C1.Enabled))
                winner = true;

            // vertical lines
            else if ((A1.Text == B1.Text) && (A1.Text == C1.Text) && (!A1.Enabled))
                winner = true;
            else if ((A2.Text == B2.Text) && (A2.Text == C2.Text) && (!A2.Enabled))
                winner = true;
            else if ((A3.Text == B3.Text) && (A3.Text == C3.Text) && (!A3.Enabled))
                winner = true;

            // diagonal lines
            else if ((A1.Text == B2.Text) && (A1.Text == C3.Text) && (!A1.Enabled))
                winner = true;
            else if ((A3.Text == B2.Text) && (A3.Text == C1.Text) && (!A3.Enabled))
                winner = true;


            if (winner)
            {
                // make sure the user can't continue to play
                disable_buttons();

                if (user_turn)
                {
                    ai_score_box.Text = (++ai_score).ToString();
                    if (MessageBox.Show("You lost.\nWas expected really...",
                        "Haha Ha!", MessageBoxButtons.OKCancel) == DialogResult.OK)
                    {
                        resetButtonsAndTurns();
                    }
                }
                else
                {
                    user_score_box.Text = (++user_score).ToString();
                    if (MessageBox.Show("You won??\n\n\nWanna test you luck again?",
                        "Hmmm", MessageBoxButtons.OKCancel) == DialogResult.OK)
                    {
                        resetButtonsAndTurns();
                    }
                }
            }
            else
            {
                if (turn_count == 9)
                {
                    draws_score_box.Text = (++draws).ToString();
                    if (MessageBox.Show("It is a draw!\nYou were a worthy adversary!\n\n\nTry again?",
                        "Congrats!", MessageBoxButtons.OKCancel) == DialogResult.OK)
                    {
                        resetButtonsAndTurns();
                    }
                }
            }
        }

        // disable all buttons
        private void disable_buttons()
        {
            /**
             * Use a try catch to prevent 
             * "Unable to cast object of type 'System.Windows.Forms.Label' 
             * to type 'System.Windows.Forms.Button'. "
             */

            foreach (Control c in Controls)
            {
                // will fail when trying to convert a label to a button
                // but will continue trying to do the rest of the controls
                try
                {
                    Button b = (Button)c;
                    b.Enabled = false;
                }
                catch {  /* ignored*/ }
            }
        }

        // will reset all our buttons, the turn_count and who's turn it is
        // the scores will remain intact
        private void newGameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("This will end your current game!", "Warning",
                MessageBoxButtons.OKCancel) == System.Windows.Forms.DialogResult.OK)
            {
                resetButtonsAndTurns();
            }               
        }


        private void resetButtonsAndTurns()
        {
            user_turn = true;       // always the user will be first
            turn_count = 0;

            foreach (Control c in Controls)
            {
                // will fail when trying to convert a label to a button
                // but will continue trying to do the rest of the controls
                try
                {
                    Button b = (Button)c;
                    b.Enabled = true;   // make sure all buttons are enabled
                    b.Text = "";        // each will contain blank
                }
                catch { /* ignored*/ }
            }
        }


        private void resetScoresToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // print a warning message and let the user choose if he really
            // wants to end the current game and reset all scores
            if (MessageBox.Show("This will end your current game!", "Warning", 
                MessageBoxButtons.OKCancel) == System.Windows.Forms.DialogResult.OK)
            {
                // reset all buttons, turn_count and who's turn is it
                resetButtonsAndTurns();

                // reset all scores and make them visible to the user
                user_score = ai_score = draws = 0;
                user_score_box.Text = (user_score).ToString();
                ai_score_box.Text = (ai_score).ToString();
                draws_score_box.Text = (draws).ToString();
            }
        }

        private void label5_Click(object sender, EventArgs e)
        {

        }
    }
}
