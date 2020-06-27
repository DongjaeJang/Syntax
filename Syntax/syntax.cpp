#include "syntax.h"
#include "table.h"

/* 
���ڿ� �迭 s :
	symbol�� �ش��ϴ� column ��ȣ�� �˱� ���� ������ ���ڿ� �迭 
	������ ���� SLR table�� column ������� symbol�� �ۼ����־���.
*/
string s[36] = { "vtype", "id", "semi", "assign", "lparen", "rparen", "lbrace", "rbrace", "comma", "if", "while", "for", "else", "literal",
				"addsub", "multdiv", "num", "float", "comp", "return", "$", "CODE", "VDECL", "ASSIGN", "FDECL", "ARG", "MOREARGS", "BLOCK", 
				"STMT",	"ELSE", "RHS", "EXPR", "TERM", "FACTOR", "COND", "RETURN" };
/*
������ �迭 before :
	reduce�� ������ �� ����Ǳ� �� viable prefix�� ���̸� production�� �������� ��Ÿ����.
	���� after�� �̿��� viable prefix�� �������ش�.
*/
int before[33] = { 0, 2, 2, 0, 3, 3, 3, 9, 3, 0, 4, 0, 2, 0, 1, 2, 8, 7, 11, 4, 0, 1, 1, 3, 1, 3, 1, 3, 1, 1, 1, 3, 3};
/*
���ڿ� �迭 after :
	reduce�� ������ �� ����� viable prefix�� ����
	���� before�� index ������ ����
*/
string after[33] = { "", "CODE", "CODE", "CODE", "VDECL", "VDECL", "ASSIGN", "FDECL", "ARG", "ARG", "MOREARGS", "MOREARGS",
					"BLOCK", "BLOCK", "STMT", "STMT", "STMT", "STMT", "STMT", "ELSE", "ELSE", "RHS", "RHS", "EXPR", "EXPR",
					"TERM", "TERM", "FACTOR", "FACTOR", "FACTOR", "FACTOR", "COND", "RETURN" };

/*
lexical analyzer ���� �� ������� output(��ū ��)�� input���� �޾� �����Ѵ�.
*/
void syntaxAnalyzer(const string& inputFile)
{
	// accept ���θ� �����ֱ� ���� bool Ÿ�� ����
	bool acceptance = true;
	
	char line[10] = { 0 };
	ifstream input(inputFile, fstream::in);

	// output ������ ������ �ʾ��� �� ���� �޼��� �߻�
	if (!input.is_open()) {
		cout << "error while opening the file\n";
		exit(0);
	}
	// output ������ ������ ���
	else {
		// ���� ������Ʈ�� ��Ÿ���� ����. startState�� 0�̹Ƿ� 0���� ����.
		int currentState = 0;
		// state�� ��ȭ�� stack�� ��� ���� ������ stack
		stack<int> state;
		/* 
		symbol�� ������� tempsymbol�� symbol vector. tempsymbol�� ��ū�¿��� �ʿ��� �κи� ��� ���� ���İ��� ���� vector��. ������ �ڵ尡
		���ư� �� ���� ���� symbol vector.
		*/
		vector<string> tempsymbol;
		vector<string> symbol;
		// viable prefix�� ���� vector
		vector<string> viable;

		// input���� ���� ������ �ٸ��� �Է¹޾� tempsymbol�� �ִ´�. tempsymbol�� ���� �� �� ex) <vtype>, <id>, <for> .....
		while (input) {
			char line[20];
			input.getline(line, 20);
			
			tempsymbol.push_back(line);
		}
		// tempsymbol�� �� ������ ���� pop����
		tempsymbol.pop_back();
		
		// tempsymbol�� ����� �� �� < �� > �� �����ϰ� ���� �κи� symbol�� �����Ѵ�.
		for (int i = 0; i < tempsymbol.size(); i++) {
			int slen = tempsymbol[i].length();
			string temp = tempsymbol[i].substr(1, slen-2);
			
			symbol.push_back(temp);
		}
		// endmarker�� symbol�� �߰�
		symbol.push_back("$");

		/*
		reduce�� ������ ����Ǿ����� �˱� ���� bool Ÿ�� ������ �����Ѵ�.
		reduce �� true��� symbol�� �� �� element�� �ѹ� �� �������ֱ� ����
				  false��� �߰� ���ž��� �����Ѵ�.
		*/
		bool reduce = false;

		state.push(currentState);
		while (true)
		{
			// stack�� �� ���κ��� currentState�� �ȴ�. top�� table�� row�κ�
			int top = state.top();
			// tempSymbol�� nextSymbol�� �ȴ�.
			string tempSymbol = symbol.front();
			int symbolNum;

			// tempSymbol�� ���ڿ� �迭 s�� element�� ���ؼ� index�� ã�� ����. symbolNum�� table�� col�κ�
			for (int i = 0; i < 36; i++)
			{
				if (s[i] == tempSymbol)
					symbolNum = i;
			}
			// ������ ������ �κ�. ���� �ൿ�� reduce���ٸ� symbol���� �� �� element�� �������ش�.
			if (reduce == true)
			{
				symbol.erase(symbol.begin());
				reduce = false;
			}

			// currentState�� currentSymbol�� ���
			cout << "top :" << top << ", symbol : " << tempSymbol << endl;
			/*
			change = table�� element
			change�� ������ ���� shift, reduce, goto, accept�� ������.
			�ش� ������ table.h �� ��������.
			*/
			int change = table[top][symbolNum];
			if (change)
			{
				// accept
				if (change == 10000)
					break;
				// reduce
				else if (change >= 1000)
				{
					// production ��ȣ�� �˱� ����.
					int production = change - 1000;
					// ����Ǿ���ϴ� viable prefix�� ũ�⸦ �޾ƿ��� ���� poplength ����
					int poplength = before[production];

					// poplength ũ�⸸ŭ viable prefix�� ����Ǿ� �ִ� vector���� �������ְ�, stack�� ����Ǿ��ִ� state�� �������ش�.
					for (int k = 0; k < poplength; k++) {
						viable.pop_back();
						state.pop();
					}
					// ����� symbol�� after[production]�� �����Ƿ� viable vector�� �� �ڿ� �߰����ش�.
					viable.push_back(after[production]);
					// reduce�� ����Ǿ����� �˷��ֱ� ���� true�� ����
					reduce = true;
					
					/* 
					if���� �����ϰ��� symbol�� ù��°�� ���� ������ �Ͼ�Ƿ� symbol���� after[production]�� �߰����ش�.
					������ reduce�� true�� �� symbol�� �� ���� �ѹ� �� �������ش�.
					*/
					symbol.insert(symbol.begin(), after[production]);
					// reduce�� ���罺����Ʈ�� ��ȭ�� �����Ƿ� stack�� push���� �ʴ´�.
				}
				// shift
				else if (change >= 100)
				{
					// �̵��� state�� ��ȣ
					currentState = change - 100;
					// viable prefix�� symbol�� �߰��Ѵ�.
					viable.push_back(tempSymbol);
					// symbol�� �� �� ���Ҹ� �����ش�.
					symbol.erase(symbol.begin());
					// state stack�� state�� �߰����ش�.
					state.push(currentState);
				}
				// goto
				else
				{
					// �̵��� state�� ��ȣ
					currentState = change;
					// state stack�� state�� �߰����ش�.
					state.push(change);
				}

			}
			// table�� element�� 0�� ��� �̵��� �� ���ٴ� ���̹Ƿ� �߸��� ��ū�� �������� ���Ѵ�. ���� while�� ����
			else {
				acceptance = false;
				break;
			}
		}
	}
	// acceptance : true => Accept ���, false => Reject ���
	if (acceptance)
		cout << "Accept" << endl;
	else
		cout << "Reject" << endl;
}