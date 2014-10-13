#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>
#include <boost/lexical_cast.hpp>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Widget.H>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <vector>

using namespace std;
class BinaryNode;
struct Point
{
	int x;
	int y;
	Point(int xx, int yy) : x(xx), y(yy)	{}
};
class TreeOperation
{
protected:
	friend class BinaryNode;
	queue<Point> points;
	queue<BinaryNode*> nodes;
	vector<int> counter;
public:
	virtual void processNode(int e, int s, int balance, std::ostream& os) = 0;
};
class BinaryNode
{
	private:
		friend class BinarySearchTree;
		bool visited;
		int element;
		int padding;
		int search_cost;
		BinaryNode *left, *right;
		void setCost(int x) { search_cost = x; }
		void setPadding(int x) { padding = x; }

	public:
		BinaryNode(int e = 0, int s = 0, BinaryNode *l = NULL,
			BinaryNode *r = NULL) :
			element(e), search_cost(s), left(l), right(r), visited(false), padding(0)	{ }
		BinaryNode *getLeft() { return left; }
		BinaryNode *getRight() { return right; }
		int getElem() { return element; }
		int getCost() { return search_cost; }
		int getPad() { return padding; }
		int size(BinaryNode *n);
		int height(BinaryNode *n);
		BinaryNode *copy();

		/* Traversal Functions */
		void preOrderTraversal(TreeOperation& op, std::ostream& os);
		void inOrderTraversal(TreeOperation& op, std::ostream& os, int balance);
		void guiInOrderTraversal(TreeOperation& op, int x, int w, int h, int t_h, std::ostream& os, int balance);
		void postOrderTraversal(TreeOperation& op, std::ostream& os);
};




/*

			



			SPLIT





*/


class DuplicateItem{};
class ItemNotFound{};
class EmptyTree{};





class BinarySearchTree
{
	private:
		int total_search_cost, depth_counter;
		BinaryNode *root;
		BinaryNode *findMin(BinaryNode *n);
		BinaryNode *find(int x, BinaryNode *n);
		BinaryNode *insert(int x, BinaryNode *n, 
			int depth = 1, std::ostream& os = std::cout);
		BinaryNode *removeMin(BinaryNode *n);
		BinaryNode *remove(int x, BinaryNode *n);
		void deleteTree(BinaryNode *n);

	public:
		BinarySearchTree() : total_search_cost(0), depth_counter(0) { root = NULL; }
		~BinarySearchTree() { deleteTree(root); root = NULL; }
		BinaryNode *getRoot() { return root; }
		bool isEmpty() { return root == NULL; }
		int size() { return (root == NULL) ? 0 : root->size(root); }
		int height() { return (root == NULL) ? 0 : root->height(root); }
		int get_total_cost() { return total_search_cost; }
		void levelByLevelOutput(std::ostream& os);
		void insertFromFile(const char *in_file, std::ostream& os = std::cout);
		void insert(int x) 
		{	root = insert(x, root);	}
		void remove(int x) 
		{	root = remove(x, root);	}
		void clear()
		{
			deleteTree(root);
			root = NULL;
			total_search_cost = 0;
			depth_counter = 0;
		}
		BinaryNode *find(int x) 
		{	return find(x, root);	}
		void copy(BinarySearchTree& rbt)
		{
			if (this != &rbt)
			{
				deleteTree(root);
				if (rbt.root != NULL) root = rbt.root->copy(); 
			}
		}
};


/*


					SPLIT!!!!!!




*/


class BalanceChoiceWidget : public Fl_Group
{
	private:
		int first_button_x;
		int second_button_x;
		int button_w;
		int spacer;
		int button_h;
		Fl_Output in_file;
		Fl_Button browse_button;
		Fl_Radio_Round_Button avl_button;
		Fl_Radio_Round_Button rb_button;
		Fl_Radio_Round_Button tf_button;
		static void browse_button_cb(Fl_Widget *v, void*);
		inline void browse_cb();

	public:
		BalanceChoiceWidget(int x, int y, int w, 
			int h, const char *l = "Balancing Methods:");
		int const get_balancing_method();
		const char *get_input_file(); 
		void draw();
};

class BinaryTreeDisplay : public Fl_Group
{
private:
	friend class PrintNodeWidget;
	BinarySearchTree bst;
	int tree_height;
	int choice;
	string tot_nodes;
	string avg_cost;
	void update_bst_display();

public:
	BinaryTreeDisplay(int x, int y, int w, int h) :
		Fl_Group(x, y, w, h), bst(), tree_height(0) {	}
	void initialize(const char* in, int choice);
	void draw();
};

class LeftUI : public Fl_Group
{
	private:
		stringstream desc_text;
		Fl_Multiline_Output description;
		BalanceChoiceWidget input;
		BinaryTreeDisplay* tree_display;
		Fl_Button go_button;
		char input_file[100];
		char *output_file;
		int balance_choice;
		static void go_button_cb(Fl_Widget *v, void*);
		inline void go_cb();

	public:
		LeftUI(int x, int y, int w, int h);
		void set_display(BinaryTreeDisplay& display);
		void draw();
};


class BinaryNodeWidget : public Fl_Widget
{
private:
	int x;
	int y;
	string element;

public:
	BinaryNodeWidget(int xx, int yy, int e) :
		x(xx), y(yy), element(boost::lexical_cast<string>(e)), Fl_Widget(xx, yy, 32, 32) {}
	void draw();
};

class BinaryLineWidget : public Fl_Widget
{
private:
	queue<Point> queued_points;
public:
	BinaryLineWidget(int x, int y, int w, int h, queue<Point> qp);
	void draw();
};

class PrintNodeWidget : public TreeOperation
{
private:
	friend class BinaryNode;
	BinaryTreeDisplay *locator;

public:
	PrintNodeWidget(BinaryTreeDisplay* address) : locator(address) { counter = vector<int>(5, 0); }
	void processNode(int e, int s, int balance, std::ostream& os);
	void processLines();
};

class PrintTreeNode : public TreeOperation
{
public:
	void processNode(int e, int s, int balance = 0, std::ostream& os = std::cout)
	{
		os << e << "[" << s << "]" << " ";
	}
};


/*



					WORK!!!!!




*/


int main()
{
	Fl_Double_Window win(100, 100, 900, 600, "Testing FLTK With Binary Nodes");
		LeftUI left_ui_input(0, 0, 260, 600);
		BinaryTreeDisplay tree_display(260, 0, 640, 360);
		left_ui_input.set_display(tree_display);
	win.end();
	win.show();
	return Fl::run();
}




int BinaryNode::size(BinaryNode *n)
{
	if (n == NULL)
		return 0;
	else return 1 + size(n->left) + size(n->right);
}

int BinaryNode::height(BinaryNode *n)
{
	if (n == NULL)
		return -1;
	else
	{
		int left_height = height(n->left);
		int right_height = height(n->right);
		return (left_height > right_height) ? 
			(1+left_height) : (1+right_height);
	}
}

BinaryNode *BinaryNode::copy()
{
	BinaryNode *root = new BinaryNode(element, search_cost);
	if (left != NULL)
		root->left = left->copy();
	if (right != NULL)
		root->right = right->copy();
	return root;
}

void BinaryNode::preOrderTraversal(TreeOperation& op, std::ostream& os = std::cout)
{
	op.processNode(element, search_cost, 0, os);
	if (left != NULL) left->preOrderTraversal(op, os);
	if (right != NULL) right->preOrderTraversal(op, os);
}

void BinaryNode::inOrderTraversal(TreeOperation& op, std::ostream& os = std::cout, int balance = 0)
{

	if (left != NULL) left->inOrderTraversal(op, os, balance);
	op.processNode(element, search_cost, balance, os);
	if (right != NULL) right->inOrderTraversal(op, os, balance);
}

void BinaryNode::guiInOrderTraversal(TreeOperation& op, int xx, int w, int h, int t_h, std::ostream& os = std::cout, int balance = 0)
{
	if (!visited)
	{
		int depth = search_cost - 1;
		int x_part = int((w / pow(2, depth)) / 2);
		int y_part = int((h / (t_h + 1)) / 2);
		int x = xx + (x_part * (2 * (op.counter[depth] + getPad()) + 1));
		int y_top = 2 * depth*y_part + y_part - 16;
		int y_bot = y_top + 32;
		if (left != NULL || right != NULL)
		{
			int y_top_lower = 2 * (depth + 1)*y_part + y_part - 16;
			if (left != NULL)
			{
				int left_x = xx + (int((w / pow(2, depth + 1)) / 2) * (2 * (op.counter[depth + 1] + left->getPad()) + 1));
				op.points.push(Point(x, y_bot));
				op.points.push(Point(left_x, y_top_lower));
			}
			if (right != NULL)
			{
				int right_x = xx + (int((w / pow(2, depth + 1)) / 2) * (2 * (op.counter[depth + 1] + right->getPad()) + (((depth + 1) == t_h && op.counter[depth + 1] == 0) ? 1 : 3)));
				op.points.push(Point(x, y_bot));
				op.points.push(Point(right_x, y_top_lower));
			}
		}
		visited = true;
	}
	if (left != NULL) left->guiInOrderTraversal(op, xx, w, h, t_h, os, balance);

	op.processNode(element, search_cost, balance, os);
	if (right != NULL) right->guiInOrderTraversal(op, xx, w, h, t_h, os, balance);
}

void BinaryNode::postOrderTraversal(TreeOperation& op, std::ostream& os = std::cout)
{
	if (left != NULL) left->postOrderTraversal(op, os);
	if (right != NULL) right->postOrderTraversal(op, os);
	op.processNode(element, search_cost, 0, os);
}


/*





				SPLIT!!!!



*/





BinaryNode *BinarySearchTree::findMin(BinaryNode *n)
{
	if (n == NULL) throw EmptyTree();
	while (n->left != NULL) n = n->left;
	return n;
}

BinaryNode *BinarySearchTree::find(int x, BinaryNode *n)
{
	while (n != NULL)
		if (x < n->element) n = n->left;
		else if (x > n->element) n = n->right;
		else return n;
	
	throw ItemNotFound();
}

BinaryNode *BinarySearchTree::insert(int x, BinaryNode *n, int depth_counter, std::ostream& os)
{
	if (n == NULL)
	{
		n = new BinaryNode(x, ++depth_counter);
		total_search_cost += n->search_cost;
		os << "Key = " << x << "   SearchCost = "
			<< depth_counter << std::endl;
	}
	else if (x < n->element)
		n->left = insert(x, n->left, ++depth_counter, os);
	else if (x > n->element)
		n->right = insert(x, n->right, ++depth_counter, os);
	else
		throw DuplicateItem();
	return n;
}

void BinarySearchTree::insertFromFile(const char *in_file, std::ostream& os)
{
	int temp_elem;
	std::ifstream ifs(in_file);
	if (ifs.fail())
	{
		std::cerr << "ERROR OPENING FILE" << std::endl 
			<< "INPUT NOT INSERTED!" << std::endl;
		return;
	}

	os << "Input data:" << std::endl;
	while (ifs >> temp_elem)	os << temp_elem << std::endl;
	ifs.clear();
	ifs.seekg(0, std::ios::beg);

	os << std::endl << "Create a binary search tree:" << std::endl;
	while (ifs >> temp_elem)	root = insert(temp_elem, root, 0, os);
}

BinaryNode *BinarySearchTree::removeMin(BinaryNode *n)
{
	if (n == NULL) throw ItemNotFound();
	if (n->left != NULL)
		n->left = removeMin(n->left);
	else
	{
		BinaryNode *temp = n;
		n = n->right;
		total_search_cost -= temp->search_cost;
		delete temp;
	}
	return n;
}

BinaryNode *BinarySearchTree::remove(int x, BinaryNode *n)
{
	if (n == NULL) throw ItemNotFound();
	if (x < n->element)
		n->left = remove(x, n->left);
	else if (x > n->element)
		n->right = remove(x, n->right);
	else if (n->left != NULL && n->right != NULL)
	{
		n->element = findMin(n->right)->element;
		n->right = removeMin(n->right);
		n->right->setCost(n->search_cost + 1);
	}
	else
	{
		BinaryNode *temp = n;
		n = (n->left != NULL) ? n->left : n->right;
		n->setCost(n->search_cost - 1);
		total_search_cost -= temp->search_cost;
		delete temp;
	}
	return n;
}

void BinarySearchTree::deleteTree(BinaryNode *n)
{
	if (n == NULL) return;
	if (n->left != NULL)
		deleteTree(n->left);
	if (n->right != NULL)
		deleteTree(n->right);
	total_search_cost -= n->search_cost;
	delete n;
}

// Based on breadth-first-search algorithm 
void BinarySearchTree::levelByLevelOutput(std::ostream& os = std::cout)
{
	os << std::endl << "Output the tree level-by-level: " << std::endl;
	std::queue<BinaryNode*> q;
	q.push(root);
	int depth = 0;
	int i = 0;
	stringstream output;
	BinaryNode *temp_node;
	while (!q.empty())
	{ 
		temp_node = q.front();
		q.pop();
		if (temp_node == NULL)	output << "X ";
		else output << temp_node->element << " ";

		if (depth < height())
			if (temp_node != NULL)
			{
				q.push(temp_node->left);
				q.push(temp_node->right);
			}
			else
			{
				q.push(NULL);
				q.push(NULL);
			}

		if (++i == int(pow(2, depth)))
		{
			output << std::endl;
			++depth;
			i = 0;
		}

	}
	char temp_c_string[50];
	stringstream temp_sstring;
	while (output.getline(temp_c_string, 50))
	{
		os << temp_c_string << endl;
		int pad = 0;
		temp_sstring.str(temp_c_string);
		while (temp_sstring >> temp_c_string)
		{
			if (temp_c_string[0] == 'X')
			{
				++pad;
				continue;
			}
			else if (temp_c_string[0] == '\n'
				|| temp_c_string[0] == '\0' || temp_c_string[0] == ' ') continue;
			else if (pad > 0)
			{
				int temp_num = atoi(temp_c_string);
				this->find(temp_num, root)->setPadding(pad);
			}
		}
		temp_sstring.clear();
	}
	temp_node = NULL;
}



/*





		SPLIT		




*/



BalanceChoiceWidget::BalanceChoiceWidget(int x, int y, int w, int h, const char *l) :
	Fl_Group(x, y, w, h, l),
	spacer(5),
	first_button_x(int(x + 3 * spacer)),
	button_w(int((w - (20 * spacer)) / 3)),
	button_h(20),
	second_button_x(first_button_x + button_w + spacer),
	in_file(first_button_x + 65, y + 10, 140, 30, "Input File:"),
	browse_button(x + int(w / 3) - 10, in_file.y() + in_file.h() + 2, 
		55, 20, "Browse"),
	avl_button(x + 2 * spacer, browse_button.y() + browse_button.h() + 20, 
		int(3 * button_w / 4), button_h, "AVL"),
		rb_button(avl_button.x() + button_w - spacer, browse_button.y() + browse_button.h() + 20,	button_w + spacer, button_h, "Red-Black"),
		tf_button(rb_button.x() + button_w + 4 * spacer, browse_button.y() + browse_button.h() + 20,
		button_w - 5 * spacer, button_h, "2-4")
{
	align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
	labelsize(12);
	labelfont(FL_BOLD);
	in_file.align(FL_ALIGN_LEFT);
	in_file.labelsize(12);
	in_file.labelfont(FL_BOLD);
	browse_button.callback(browse_button_cb);
	avl_button.align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
	rb_button.align(FL_ALIGN_INSIDE | FL_ALIGN_RIGHT);
	tf_button.align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
	avl_button.labelsize(12);
	rb_button.labelsize(12);
	tf_button.labelsize(12);
	end();
}

int const BalanceChoiceWidget::get_balancing_method()
{
	if (avl_button.value() == '1')	return 1;
	else if (rb_button.value() == '1')	return 2;
	else if (tf_button.value() == '1')	return 3;
	else return 0;
}

const char *BalanceChoiceWidget::get_input_file()
{
	return in_file.value();
}

void BalanceChoiceWidget::draw()
{
	Fl_Widget *const*child_widgets = array();
	if (damage() == FL_DAMAGE_CHILD)
	{
		for (int i = 0; i < children(); ++i)
		{
			update_child(**child_widgets);
			child_widgets++;
		}
	}
	else
	{
		fl_draw_box(FL_DOWN_BOX, x(), y(), w(), h(), FL_LIGHT2);
		draw_label();
		for (int j = 0; j < children(); ++j)
		{
			draw_child(**child_widgets);
			draw_outside_label(**child_widgets);
			child_widgets++;
		}

	}
}

void BalanceChoiceWidget::browse_button_cb(Fl_Widget *v, void *)
{
	((BalanceChoiceWidget*)v->parent())->browse_cb();
}

void BalanceChoiceWidget::browse_cb()
{
	in_file.value(fl_file_chooser("Input File Browser", "*", "*", 1));
}


LeftUI::LeftUI(int x, int y, int w, int h) :
Fl_Group(x, y, w, h),
output_file("output.txt"),
balance_choice(0),
description(x + 20, y + 20, w - 40, int(h / 3) + 15),
input(description.x() + 5, description.y() + 40 + description.h(),
description.w() - 10, 150),
go_button(int((w / 2) - 50), input.y() + 50 + input.h(),
100, 30, "Balance!")

{
	desc_text << "    This program reads \n integer data"
		<< " from a text\n  file and will display "
		<< "the \n generated Binary Search\n   Tree"
		<< " with the selected \n  balancing method."
		<< " This \n program only works with\n    input size "
		<< "less than or \n\t    equal to 31.";
	description.box(FL_DOWN_BOX);
	description.color(FL_BACKGROUND_COLOR);
	description.value(desc_text.str().c_str());
	description.textsize(20);
	description.textfont(FL_TIMES_BOLD);
	description.textcolor(fl_rgb_color(80, 0, 0));
	go_button.callback(go_button_cb);
	end();
}

void LeftUI::draw()
{	
	Fl_Widget *const*child_widgets = array();
	if (damage() == FL_DAMAGE_CHILD)
	{
		for (int i = 0; i < children(); ++i)
		{
			update_child(**child_widgets);
			child_widgets++;
		}
	}
	else
	{
		fl_draw_box(FL_UP_BOX, x(), y(), w(), h(), FL_DARK2);
		for (int j = 0; j < children(); ++j)
		{
			draw_child(**child_widgets);
			child_widgets++;
		}
	}
}

void LeftUI::go_button_cb(Fl_Widget *v, void*)
{
	((LeftUI*)v->parent())->go_cb();
}

void LeftUI::go_cb()
{
	strncpy(input_file, input.get_input_file(), 100);
	balance_choice = input.get_balancing_method();
	tree_display->initialize(input_file, balance_choice);
}

void LeftUI::set_display(BinaryTreeDisplay& display)
{
	tree_display = &display;
}

void BinaryNodeWidget::draw()
{
	Fl_Color old_color = fl_color();
	int center_spacing = 4 * element.size() - 2;
	fl_color(80, 0, 0);
	fl_circle(x, y, 16);
	fl_draw(element.c_str(), x - center_spacing, y + 5);
	fl_color(old_color);
}

void PrintNodeWidget::processNode(int e, int s, int balance, std::ostream& os = std::cout)
{
	os << e << "[" << s << "]" << " ";
	int depth = s - 1;
	int x_part = int((locator->w() / pow(2, depth)) / 2);
	int y_part = int((locator->h() / (locator->tree_height + 1)) / 2);
	int x = locator->x() + (x_part * (2 * (counter[depth] + locator->bst.find(e)->getPad()) + 1));
	int y = 2 * depth*y_part + y_part;
	locator->add(new BinaryNodeWidget(x, y, e));
	++counter[depth];
}

void PrintNodeWidget::processLines()
{
	locator->add(new BinaryLineWidget(locator->x(), locator->y(), locator->w(), locator->h(), points));
}

void BinaryTreeDisplay::initialize(const char* in, int c)
{
	bst.clear();
	choice = c;
	bst.insertFromFile(in, cout);
	if (bst.size() <= 31)
		update_bst_display();
	else
	{
		clear();
		PrintTreeNode pt;
		cout << endl << "In-Order Traversal: (Nodes represented as Key[SearchCost])" << endl;
		bst.getRoot()->inOrderTraversal(pt, cout, 0);
		cout << endl << "Total number of nodes: " << bst.size() << endl
			<< "Average search cost = total sum of node search cost / total nodes "
			<< endl << " = " << bst.get_total_cost() << " / " << bst.size() << " = "
			<< (bst.get_total_cost() / (double)bst.size()) << endl;
		bst.levelByLevelOutput(cout);
	}
}

void BinaryTreeDisplay::update_bst_display()
{
	begin();
	clear();
	tree_height = bst.height();
	PrintNodeWidget pt(this);
	bst.levelByLevelOutput(cout);
	bst.getRoot()->guiInOrderTraversal(pt,x(), w(), h(), tree_height, cout, choice);
	pt.processLines();
	tot_nodes = "Total Number of Nodes:   " + boost::lexical_cast<string>(bst.size());
	string aver = boost::lexical_cast<string>(bst.get_total_cost() / double(bst.size()));
	avg_cost = "Average Search Cost:  " + boost::lexical_cast<string>(bst.get_total_cost()) + " / " + boost::lexical_cast<string>(bst.size()) + " = " + aver.substr(0, 6);
	add(new Fl_Box(FL_NO_BOX, x() + 10, y() + 10, 200, 20, tot_nodes.c_str()));
	add(new Fl_Box(FL_NO_BOX, x() + 10, y() + 30, 277, 20, avg_cost.c_str()));
	redraw();
	end();
}

void BinaryTreeDisplay::draw()
{
	Fl_Widget *const*child_widgets = array();
	if (damage() == FL_DAMAGE_CHILD)
	{
		for (int i = 0; i < children(); ++i)
		{
			update_child(**child_widgets);
			child_widgets++;
		}
	}
	else
	{
		fl_draw_box(FL_DOWN_BOX, x(), y(), w(), h(), FL_WHITE);
		for (int j = 0; j < children(); ++j)
		{
			draw_child(**child_widgets);
			draw_outside_label(**child_widgets);
			child_widgets++;
		}
	}
}

BinaryLineWidget::BinaryLineWidget(int x, int y, int w, int h, queue<Point> qp) :
Fl_Widget(x, y, w, h),
queued_points(qp)	{	}

void BinaryLineWidget::draw()
{
	Fl_Color old_color = fl_color();
	fl_color(fl_rgb_color(80, 0, 0));
	fl_line_style(FL_SOLID, 2);
	while (!queued_points.empty())
	{
		int x = queued_points.front().x;
		int y = queued_points.front().y;
		queued_points.pop();
		if (queued_points.empty()) break;
		int next_x = queued_points.front().x;
		int next_y = queued_points.front().y;
		queued_points.pop();
		fl_line(x, y, next_x, next_y);
	}
}
