#include <iostream>
#include <fstream>
#include <cstdlib>

/*
Node to traverse through binary tree. 
    If node represents a question then question_or_name
    will be a question and if_yes/if_no will be a pointer to 
    either another question or an animal (if at the end of the tree)

    If node represents an animal then question_or_name
    will be the name of the animal and if_yes/if_no will be NULL pointers.

    The users input to questions will navigate through these pointers 
    until it finds a node with NULL pointers, indicating the node is an animal
    and then it will guess that animal. 
        If the guess is wrong then the users new input question will create a new
        question node where if_no will point to the previous guess and if_yes will
        point to the new animal that was just named by the user

        If the guess is correct then we win!
*/
struct Node
{
    std::string question_or_name;
    Node *if_yes, *if_no;
};

// Function prototypes
// if no save file detected, start new
void starting_binary_tree(Node*& current_node);
// ask for response (yes/no) from user
bool query_response();
// start loop of asking user questions
void ask_questions(Node*& start, Node*& previous_node);
// after loop, ask if we got the animal right
bool ask_if_animal(Node* start);
// if we did not, build a new question
void build_question(Node*& start);

// used for saving / checking if save exists.
// creates/reads save file named "save_file.txt"
// which is stored where you run the binary from
bool check_file(std::ifstream& file);
void read_in_file(Node*& current_node, std::ifstream& file);
void write_to_file(Node* current_node, std::ofstream& file);

// EXTRA CREDIT: delete tree and start over
void delete_tree(Node* node);


/*
INPUT: None
OPERATION: general flow of operation:
    1. Greet user and ask if they want to start the game
    2. Check if a save file exists (data from past gameplay)
        and read in that data if the user wants. Otherwise load 
        the base starting binary tree. 
    3. Begin playing game. Ask questions, moving through binary tree
        until you arrive at the end of a branch with only an animal 
        guess remaining
    4. Ask the user if this is the animal
    5. If it is then we win! (Skip to step 7)
    6. If it is not then ask the user for their animal and a defining
        question. Use this to build more nodes in the binary tree
    7. Ask user if they want to play again 
    8. (EXTRA CREDIT RESET OPTION) If they do, ask if they want to
        continue their game. Yes will continue with new nodes - No will
        delete the created nodes and reload the base starting binary tree 
    9. Play again. Once done playing write the tree to the save file
        so the user can pick up where they left off the next time they 
        play.
OUTPUT: None
*/
int main()
{
    Node* current_node;
    Node* previous_node;
    bool play_game;


    // saving/loading/resetting game
    std::ifstream load_file;
    std::ofstream save_file;
    bool save_available;
    bool desire_save_file;
    std::string filename = "save_file.txt";
    bool continue_game;

    // greeting, start game
    std::cout << "Hello, do you want to play a game?\n" << 
        "Possible reponses:\nyes | Yes | YES | y\nno | No | NO | n\n\n" << std::endl;
    play_game = query_response();

    // check for save file. Either load file 
    // or use starting_binary_tree.
    desire_save_file = check_file(load_file);
    if (desire_save_file){
        read_in_file(current_node, load_file);
    }
    else{
        starting_binary_tree(current_node);
    }

    // start game
    while (play_game)
    {
        // loop through questions in node tree
        ask_questions(current_node, previous_node);

        // ask if final node is the animal the user chose
        if (ask_if_animal(previous_node))
        {
            std::cout << "I WIN!!!!!" << std::endl;
        }
        else
        {
            std::cout << "I LOSE!!!" << std::endl;
            build_question(previous_node);
        }

        // ask if the user wants to play again 
        std::cout << "Play again?" << std::endl;
        play_game = query_response();

        // EXTRA CREDIT: reset function.
        if (play_game){
            std::cout << "Do you want to continue your game?" << 
                " (NO to load base tree again)" << std::endl;
            continue_game = query_response(); 
            if (!continue_game){
                delete_tree(current_node);
                starting_binary_tree(current_node);
            }
        }       
    }
    // save data
    save_file.open(filename);
    write_to_file(current_node,save_file);
    save_file.close();
    std::cout << "Save File Generated.\nThanks for playing! Closing now." << std::endl;


    return 0;
}
/*
warning: non-void function does not return a value in all control paths
function throws the above warning but the else-else path is an
endless loop so I don't think this can actually happen?

INPUT: Node pointer
OPERATION: Ask if the animal node we have arrived at is correct. If it is return true
    else return false. If invalid response, recall the function.
OUTPUT:boolean representing if the animal guess was correct or not
*/
bool ask_if_animal(Node* start)
{
    int user_decision;
    std::cout << "Is your animal a(n) " << start->question_or_name << "?" << std::endl;
    user_decision = query_response();
    if (user_decision == 0)
        return false;
    else
    {
        if (user_decision == 1)
            return true;
        else
        {
            std::cout << "Invalid response.";
            ask_if_animal(start);
        }
    }
}
/*
INPUT: Node pointer reference (current node), node pointer reference (previous node)
OPERATION: Ask the initial question. If yes, call the function again, this time exploring the if_yes pointer. 
    if no, call the function again this time exploring the if_no pointer. 
    Continue this operation until the if_yes/if_no pointers both point to NULL, indicating we have arrived
    at a final animal guess.
OUTPUT:None
*/
void ask_questions(Node*& cur_node, Node*& previous_node)
{
    bool user_choice;
    previous_node = cur_node;
    // check if we are at an animal
    if (cur_node->if_yes == NULL && cur_node->if_no == NULL)
        return;
    else
    {
        // we are at a question
        std::cout << "Question: " << cur_node->question_or_name << std::endl;
        user_choice = query_response();
        if (user_choice) // NO -> if_no
        {
            ask_questions(cur_node->if_yes, previous_node);
        }
        else
        {
            ask_questions(cur_node->if_no, previous_node);  
        }

    }
}
/*
INPUT: Node reference
OPERATION: The node passed in represents the animal that was guessed. 
    Ask the user what their animal was and a question that could have differentiated it from the guess. 
    Then create two new nodes to represent the yes/no animal options. The incorrect animal guess node now
    becomes a question node which points to the two new nodes repesenting old/new animal
    guesses using the if_yes/if_no pointers of the question node. 
OUTPUT: None
*/
void build_question(Node*& start)
{
    std::string new_animal;
    std::string new_question;
    std::cout << "What was your animal?" << std::endl;
    std::getline(std::cin, new_animal);

    std::cin.clear();

    std::cout << "Please give a question whose answer is YES for " << 
        new_animal << " and NO for " << start->question_or_name << "." << std::endl;

    std::getline(std::cin,new_question);

    std::cin.clear();
    
    // yes node becomes new animal
    Node* if_yes = new Node;
    if_yes->question_or_name = new_animal;
    if_yes->if_yes=NULL;
    if_yes->if_no=NULL;

    // no node becomes old animal
    Node* if_no = new Node;
    if_no->question_or_name = start->question_or_name;
    if_no->if_yes = NULL;
    if_no -> if_no = NULL;

    // cur node becomes question and points to yes/no nodes
    start->question_or_name = new_question;
    start->if_yes = if_yes;
    start->if_no = if_no;
    return;
}
/*
INPUT: None
OPERATION: create 3 nodes. root node, yes node, no node. 
    the root node question_or_name will be a question, in this case, 
    'Does your animal live primarily in water?'. 

    The if_yes pointer points to the yes node who has question_or_name
    equal to Octopus and has NULL if_yes/if_no pointers

    The if_no pointer points to the no node who has question_or_name
    equal to Bear and has NULL if_yes/if_no pointers
OUTPUT:root Node pointer
*/
void starting_binary_tree(Node*& current_node)
{
    Node* yes = new Node;
    Node* no = new Node;
    current_node = new Node;

    no->question_or_name = "Bear";
    no->if_yes = NULL;
    no->if_no = NULL;

    yes->question_or_name = "Octopus";
    yes->if_yes = NULL;
    yes->if_no = NULL;

    current_node -> question_or_name = "Does your animal live primarily in water?";
    current_node ->if_yes = yes;
    current_node -> if_no = no;

}
/*warning: non-void function does not return a value in all control paths
function throws the above warning but the else-else path is an
endless loop so I don't think this can actually happen?

Input: NONE
Operation: get user input and return true if:
 yes, Yes, YES, y
or return false if:
  no, No, NO, n
Output: boolean representing yes(true) or no(false) response to asked question.
*/
bool query_response()
{
    std::string response;
    std::cin.clear();
    std::getline(std::cin,response);

    if(response == "yes" || response == "Yes" || response == "YES" || response == "y")
    {
        return true;
    }
    else
    {
        if(response == "no" || response == "No" || response == "NO" || response == "n")
        {
            return false;
        }
        else
        {
            std::cout << "Invalid response" << std::endl;
            query_response();
        }
    }
}

/*
How saving works / Output file structure:

W/ Starting Binary Tree:
1. Does your animal live primarily in water?
2. Octopus//(if yes)
3. Null // (animal pointers null)
4. Null // (animal pointers null)
5. Bear //(if no)
6. Null // (animal pointers null)
7. Null // (animal pointers null)

After More Questions have been added:
NOTE: Marlin does Live in Water, not an Octopus, has fins.
1. Does your animal live primarily in water?
2. Does it have fins? // if yes live in water
3. Marlin // if yes live in water, yes has fins
4. Null // (animal pointers null)
5. Null // (animal pointers null)
6. Octopus //if yes live in water, no has fins
9. Null // (animal pointers null)
10. Null // (animal pointers null)
11. Bear //(if no live in water)
12. Null // (animal pointers null)
13. Null // (animal pointers null)


INPUT: ifstream reference
OPERATION: use the ifstream to read in save_file.txt
and check if the file exists. 
OUTPUT: return true if file exists, false if it does not
*/
bool check_file(std::ifstream& file){
    bool response;
    std::string filename = "save_file.txt";
    file.open(filename);
    // this will fail if the file doesn't exist.
    if (file.fail()){
        std::cout << "no save file detected. starting new game" << std::endl;
        return false;
    }
    else{
        std::cout << "save file detected. Do you want to use it?" << std::endl;
        response = query_response();
        if (response){
            return true;
        }
        else{
            return false;
        }

    }
}
/* 
INPUT: ifstream reference
OPERATION: we have already opened the file
    when checking if the file exists. First, read the first line.
    If the line contains text (the first line should) then create
    a new node and assign the current_node pointer to point to it. 
    It will make the question_or_name string equal to the read string
 OUTPUT: None
*/
void read_in_file(Node*& current_node, std::ifstream& file){
    std::string read_line;
    std::getline(file, read_line);

    if (read_line == "NULL"){
        current_node = NULL;
    }
    else{
        current_node = new Node;
        current_node->question_or_name = read_line;
        read_in_file(current_node->if_yes, file);
        read_in_file(current_node->if_no, file);
    }
}
/*
INPUT: node pointer, ostream reference
OPERATION: look at current node. If it is NULL then print NULL
    to the text file. If it is not NULL then save the question_or_node
    to text file and continue with the if_yes/if_no pointers of the node.
    This will create a file like the file that we read in the above func.
OUTPUT: None
*/
void write_to_file(Node* current_node, std::ofstream& file){
    if (current_node == NULL){
        file << "NULL" << std::endl;
    }
    else{
        file << current_node->question_or_name << std::endl;
        write_to_file(current_node->if_yes, file);
        write_to_file(current_node->if_no, file);
    }
}
/*
USED TO RESET GAME! (could also do by exiting the program and not loading your saved game)
INPUT: node pointer
OPERATION: deletes node if you want to start over by taking a node pointer
    and deleting the if_yes/if_no pointers of the node then deleting the node.
OUTPUT: None
*/
void delete_tree(Node* node){
    if (node == NULL) return;
    delete_tree(node->if_yes);
    delete_tree(node->if_no);
    delete node;
}
