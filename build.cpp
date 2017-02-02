#include <iostream>
#include <cstdlib>
#include <list>
#include <ctime>
#include <vector>

using namespace std;

namespace builder
{
	char paro;
	char parf;

	class op;

	int random(int max)
	{
		int z = std::rand() % max;
		if (z<0)
		{
			cerr << "PFFF " << z << endl;
			exit(1);
		}
		return z;
	}

	op* buildOp(int depth);

	class op
	{
		public:
			op() { }
			virtual ~op() {};
			virtual void fill(int depth)=0;
			virtual void dump() = 0;
			virtual bool isTerminal() const=0;

			virtual op* clone()=0;

			static op* buildOp(int depth);
			static op* clone(vector<op*>&);

		protected:
			static void on_a_une_nouvelle_instance(op* p)
			{
				cout << "On a une nouvelle instance " << (p->isTerminal() ? "" : "non" ) << " terminale." << endl;
				if (p->isTerminal())
					terminals.push_back(p);
				else
					not_terminals.push_back(p);
			}

		private:
			static vector<op*>	terminals;
			static vector<op*>	not_terminals;
	};
	vector<op*>	op::terminals;
	vector<op*> op::not_terminals;

	op* op::clone(vector<op*>& v)
	{
		int i=random(v.size());
		return v[i]->clone();
	}

	class op_terminal : public op
	{
		public:
			op_terminal(bool regit) { if (regit) on_a_une_nouvelle_instance(this); };
			virtual ~op_terminal() {};
			virtual bool isTerminal() const { return true; }
	};

	class op_not_terminal : public op
	{
		public:
			op_not_terminal(bool regit) { if (regit) on_a_une_nouvelle_instance(this); };
			virtual ~op_not_terminal() {};
			virtual bool isTerminal() const { return false; }
	};

	class plus : public op_not_terminal
	{
		static plus instance;
		public:
			plus(bool regit) : op_not_terminal(regit) {};
			virtual op* clone() { return new plus(false); }

			virtual void fill(int depth)
			{
				int end=random(3)+2;
				if (end <= 1)
				{
					cerr << "MOUARFF" << endl;
					exit(1);
				}
				for(int i = 0; i<end; i++)
				{
					op* p = buildOp(depth);
					args.push_front(p);
				}
			}

			virtual void dump()
			{
				if (args.size()<=1)
				{
					cerr << "MDR SIZE PLUS = " << args.size() << endl;
					exit(1);
				}
				cout << paro << "+ ";
				for(auto p : args)
					p->dump();
				cout << ' ' << parf << ' ';
			}

			list<op*>	args;
	};
	plus plus::instance(true);

	class factor : public op_terminal
	{
		static factor instance;
		public:
			factor(bool regit) : op_terminal(regit) {}

			virtual op* clone() { return new factor(false); }
			virtual void fill(int depth)
			{
				value = random(1000);
			}

			virtual void dump()
			{
				cout << ' ' << value << ' ';
			}

			int value;
	};

	class non : public op_not_terminal
	{
		static non instance;
		public:
			non(bool regit) : op_not_terminal(regit) {}
			virtual op* clone() { return new non(false); }
			virtual void fill(int depth)
			{
				expr = buildOp(depth);


			}
			virtual void dump()
			{
				cout << "!" << paro ;
				expr->dump();
				cout << parf;
			}

			op* expr;
	};
	non non::instance(true);

	factor factor::instance(true);

	op* op::buildOp(int depth)
	{
		op* p;
		if (depth == 0 || random(100)>50)
			p= clone(terminals);
		else
			p= clone(not_terminals);

		p->fill(depth-1);
		return p;
	}
}
int main(int argc, const char* argv[])
{

	int n = 3;
	builder::paro = '(';
	builder::parf = ')';

	if (argc==2)
	{
		builder::paro = '[';
		builder::parf = ']';
	}
	if (argc==3)
	{
		n = atoi(argv[1]);
		if (n<1)
			n=1;
	}
	n+=2;

	std::srand(std::time(0));
	builder::op* top = builder::op::buildOp(n);

	top->dump();
	return 0;
}

