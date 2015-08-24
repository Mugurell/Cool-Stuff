//
//  Author:           Lingurar Petru-Mugurel
//  Written:          21 Aug 2015  11:27:34
//  Last updated:           ---
//
//
//  Description:
//   Really nice Tic Tac Toe game. 
//   It also has it's own (doesn't care to win) AI !
//
//  Bugs:
//    --- None ---
//
//  TODO:
//    --- None --- 
//
//  Notes:
//    Made in 8 hourse with 0 prior experience with C# and .net framework.
//


using System;
using System.Collections.Generic;
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
        // if the user declined to play a new game after the last one we need to
        // remember the state; will be used as condition in newGameToolStripMenu()
        // and before calling flowerPowerAI in button_click
        private bool canceled_new_game = false;     

        public Form1()
        {
            InitializeComponent();
        }


        private void button_click(object sender, EventArgs e)
        {
            Button b = (Button)sender;
            if (user_turn)
            {
                b.Text = "X";

                // change the turn
                user_turn = !user_turn;

                // prevent the user to change the recorded state
                b.Enabled = false;

                // for every move increase the turn count
                ++turn_count;

                // after any player could have made 3 moves check for a winner
                if (turn_count >= 5)
                    checkForWinner();

                // now call the AI to make it's move
                // will also check for winner
                if (!user_turn && !canceled_new_game)
                    flowerPowerAI();
            }
        }



        private void checkForWinner()
        {
            bool winner = false;

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
                disableButtons();

                // if is the turn of the user -> the AI made the last move
                // and if we have a winner -> AI must be the one
                if (user_turn)
                {
                    ai_score_box.Text = (++ai_score).ToString();
                    if (MessageBox.Show("You lost.\nWas expected really...",
                        "Haha Ha!", MessageBoxButtons.OKCancel) == DialogResult.OK)
                    {
                        resetButtonsAndTurns();
                    }

                    /** if the user declined to play a new game after the last 
                     * one we need to remember the state; will be used as condition
                     * in newGameToolStripMenu() and before calling flowerPowerAI
                     * in button_click
                     */
                   else
                       canceled_new_game = true;
               }
               else    // the user had the last move -> he must be the winner
               {
                   user_score_box.Text = (++user_score).ToString();
                   if (MessageBox.Show("You won??\n\n\nWanna test you luck again?",
                       "Hmmm", MessageBoxButtons.OKCancel) == DialogResult.OK)
                   {
                       resetButtonsAndTurns();
                   }
                    else
                        canceled_new_game = true;

                }
           }
           else    // all 9 boxes filled and no winner -> tie
           {
               if (turn_count == 9)
               {
                   draws_score_box.Text = (++draws).ToString();
                   if (MessageBox.Show("It is a draw!\nYou were a worthy adversary!\n\n\nTry again?",
                       "Congrats!", MessageBoxButtons.OKCancel) == DialogResult.OK)
                   {
                       resetButtonsAndTurns();
                   }
                    else
                        canceled_new_game = true;
                }

           }
       }



       // disable all buttons
       private void disableButtons()
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



        private void resetButtonsAndTurns()
        {
            user_turn = true;       // always the user will be first
            turn_count = 0;
            canceled_new_game = false;

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


        private void flowerPowerAI()
        {
            // first construct the pool of available spaces
            List<Control> available_spaces = new List<Control>(4);

            foreach (Control c in Controls)
            {
                // will fail when trying to convert a label to a button
                // but will continue trying to do the rest of the controls
                try
                {
                    Button b = (Button)c;
                    if (b.Enabled)
                        available_spaces.Add(c);

                }
                catch { /* ignored*/ }
            }

            if (available_spaces.Count == 0)
                return;


            // get a truly random number by using a seed that is also
            // random generated; overkill?
            Random posRandom = new Random(Guid.NewGuid().GetHashCode());
            Control toBeUsed = available_spaces[posRandom.Next(0, available_spaces.Count)];

            // print O and use that random button as AI's pick
            Button b2 = (Button)toBeUsed;
            b2.Text = "O";

            // change the turn
            user_turn = !user_turn;

            // prevent the user to change the recorded state
            b2.Enabled = false;

            // for every move increase the turn count
            ++turn_count;

            // after any player could have made 3 moves check for a winner
            if (turn_count >= 5)
                checkForWinner();
        }



        private void label2_Click(object sender, EventArgs e)
        {
            //Todo: If time, clicking this will change between clever/dumb AI.
        }





        /*************************************
         * File ToolStrimMenu Buttons:
         ************************************/

        // will reset all our buttons, the turn_count and who's turn it is
        // the scores will remain intact
        private void newGameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // if the user pressed this after finishing a game,
            // there's no need to warn him about ending the current game..
            if (canceled_new_game)
            {
                resetButtonsAndTurns();
            }
            else if (MessageBox.Show("This will end your current game!", "Warning",
                MessageBoxButtons.OKCancel) == System.Windows.Forms.DialogResult.OK)
            {
                resetButtonsAndTurns();
            }
        }


        private void resetScoresToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // if the user pressed this after finishing a game,
            // there's no need to warn him about ending the current game..
            if (canceled_new_game)
            {
                resetButtonsAndTurns();
            }

            // print a warning message and let the user choose if he really
            // wants to end the current game and reset all scores
            else if (MessageBox.Show("This will end your current game!", "Warning",
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


        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Do you really want to exit?", "Warning",
                MessageBoxButtons.OKCancel) == System.Windows.Forms.DialogResult.OK)
                Application.Exit();
        }



        /************************************
         * Help ToolStrimMenu Buttons:
         ***********************************/
        private void rulesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string rules = "The object of Tic Tac Toe is to get three in a row."
                           + "\nYou play on a three by three game board."
                           + "\nThe first player is known as X and the second is O."
                           + "\nPlayers alternate placing Xs and Os on the game board until"
                           + "\neither oppent has three in a row or all nine squares are filled."
                           + "\nX always goes first, and in the event that no one has three "
                           + "\nin a row, the stalemate is called a cat game."
                           + "\n\nYou are with X's and the PC will use O's.";
            MessageBox.Show(rules, "Rules", MessageBoxButtons.OK);
        }


        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("\tBy Lingurar Petru-Mugurel\n\n\nCheck sources on Github?",
                "About TTT", MessageBoxButtons.OKCancel)
                == System.Windows.Forms.DialogResult.OK)
            {
                string git_link = "https://github.com/Mugurell/Cool-Stuff/tree"
                                  + "/master/WF%20-%20Tic%20Tac%20Toe";
                System.Diagnostics.Process.Start(git_link);
            }
        }


    }   // end class Form1 : Form

}   // end namespace WF___Tic_Tac_Toe
