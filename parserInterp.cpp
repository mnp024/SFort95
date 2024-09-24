//Michael Piscopo
//mnp24
//CS280-012
#include <queue>
#include <stack>

#include "parserInterp.h"
#include "lex.cpp"//only for vscode
#include "val.cpp"//only for vscode

map<string, bool> defVar;
map<string, bool> assignVar;
map<string, Token> SymTable;

map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

queue<string> initSuccessMessages;
queue<string> defSuccessMessages;
queue<string> ifSuccessMessages;
string initialized;
LexItem tempTok;
int nestLevel = 0;
bool runCode = true; //for if statements (set false to not run)


namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line);

// //PrintStmt:= PRINT *, ExpreList 
// bool PrintStmt(istream& in, int& line) {
// 	LexItem t;
	
// 	t = Parser::GetNextToken(in, line);
	
//  	if( t != PRINT ) {
		
// 		ParseError(line, "Print statement syntax error.");
// 		return false;
// 	}

// 	t = Parser::GetNextToken(in, line);
	
//  	if( t != DEF ) {
		
// 		ParseError(line, "Print statement syntax error.");
// 		return false;
// 	}
// 	t = Parser::GetNextToken(in, line);
	
// 	if( t != COMMA ) {
		
// 		ParseError(line, "Missing Comma.");
// 		return false;
// 	}
// 	bool ex = ExprList(in, line);
	
// 	if( !ex ) {
// 		ParseError(line, "Missing expression after Print Statement");
// 		return false;
// 	}
// 	return ex;
// }//End of PrintStmt

// ^ OLD PRINTSTMT
// v NEW PRINTSTMT

//PrintStmt:= PRINT *, ExpreList 
bool PrintStmt(istream& in, int& line) {
	LexItem t;
	ValQue = new queue<Value>;
	
	
	t = Parser::GetNextToken(in, line);
	
 	if( t != PRINT ) { 
		
 		ParseError(line, "Print statement syntax error.");
 		return false;
 	}

	t = Parser::GetNextToken(in, line);

 	if( t != DEF ) {
		
		ParseError(line, "Print statement syntax error.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	
	if( t != COMMA ) {
		
		ParseError(line, "Missing Comma.");
		return false;
	}
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after Print Statement");
		return false;
	}
	
	while (!(*ValQue).empty() && runCode)
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	if(runCode){
		cout << endl;
	}
	return ex;
}//End of PrintStmt

// //ExprList:= Expr {,Expr}
// bool ExprList(istream& in, int& line) {
// 	bool status = false;
	
// 	status = Expr(in, line);
// 	if(!status){
// 		ParseError(line, "Missing Expression");
// 		return false;
// 	}
	
// 	LexItem tok = Parser::GetNextToken(in, line);
	
// 	if (tok == COMMA) {
// 		status = ExprList(in, line);
// 	}
// 	else if(tok.GetToken() == ERR){
// 		ParseError(line, "Unrecognized Input Pattern");
// 		cout << "(" << tok.GetLexeme() << ")" << endl;
// 		return false;
// 	}
// 	else{
// 		Parser::PushBackToken(tok);
// 		return true;
// 	}
// 	return status;
// }//End of ExprList

// ^ OLD EXPRLIST
// v NEW EXPRLIST

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value retVal;
	
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	if(runCode){
		ValQue->push(retVal);
	}
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
		
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//Prog:= PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line) {

	LexItem t;
	
	t = Parser::GetNextToken(in, line);
	
 	if( t != PROGRAM ) {
		
		ParseError(line, "Program syntax error.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	
	if( t != IDENT ) {
		
		ParseError(line, "Missing Program name");
		return false;
	}
	LexItem progName = t;

	t = Parser::GetNextToken(in, line);
	bool declChecks = true;
	while(t != END) {
		//decls and stmts
		if(t == INTEGER || t == REAL || t == CHARACTER){
			Parser::PushBackToken(t);
			if(!declChecks){
				ParseError(line, "Decl Statement in wrong place");
				return false;
			}
			if(!Decl(in, line)){
				ParseError(line, "Decl Statement Syntax error.");
				return false;
			}
		} else {
			Parser::PushBackToken(t);
			declChecks = false;
			if(!Stmt(in,line)){
				ParseError(line, "Not a Statement");
				return false;
			}
		}
		t = Parser::GetNextToken(in, line);
	}

	t = Parser::GetNextToken(in, line);
	if( t != PROGRAM ) {
		
		ParseError(line, "Program syntax error.");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	
	if( t != IDENT ) {
		
		ParseError(line, "Missing Program name");
		return false;
	}
	if(t.GetLexeme() != progName.GetLexeme()){
		ParseError(line, "Program names don't match");
		return false;
	}
	// while(!initSuccessMessages.empty()){
	// 	cout << initSuccessMessages.front() << endl;
	// 	initSuccessMessages.pop();
	// }
	// while(!defSuccessMessages.empty()){
	// 	cout << defSuccessMessages.front() << endl;
	// 	defSuccessMessages.pop();
	// }
	// while(!ifSuccessMessages.empty()){
	// 	cout << ifSuccessMessages.front() << endl;
	// 	ifSuccessMessages.pop();
	// }
	cout << "(DONE)" << endl;
	return true;

}// End of Prog


//Decl:= Type :: VarList
bool Decl(istream& in, int& line) {

	LexItem t;
	LexItem idtok;

	if(!Type(in,line)){
		ParseError(line, "Type syntax error.");
		return false;
	}
	
	idtok = tempTok;//set idtok to the type token
	t = Parser::GetNextToken(in, line);
	if(t != DCOLON){
		ParseError(line, "Missing :: in Decl Statement");
		return false;
	}
	if(!(TempsResults.find("strlen") == TempsResults.end())){
		Value strlenVal = TempsResults["strlen"];
		int strlen = strlenVal.GetInt();
		if(!VarList(in,line,idtok,strlen)){
			ParseError(line, "VarList missing from Decl");
			return false;
		}//plz plz plz work
	} else {
		if(!VarList(in,line,idtok)){
			ParseError(line, "VarList missing from Decl");
			return false;
		}
	}
	// if(!VarList(in,line,idtok)){
	// 	ParseError(line, "VarList missing from Decl");
	// 	return false;
	// }
	return true;

}// End of Decl


//Type:= INTEGER | REAL | CHARACTER [(LEN = ICONST)]
bool Type(istream& in, int& line) {
	LexItem t;
	t = Parser::GetNextToken(in, line);

	if(t == INTEGER || t == REAL){
		if(!(TempsResults.find("strlen") == TempsResults.end())){
			TempsResults.erase("strlen"); //so strlen doesnt get passed when not char
		}
		tempTok = t;
		return true;
	}
	if(t != CHARACTER){
		return false;
	}
	tempTok = t;
	t = Parser::GetNextToken(in, line);

	if(t != LPAREN){
		Parser::PushBackToken(t);
		return true;
	}
	t = Parser::GetNextToken(in, line);
	if(t != LEN){
		ParseError(line, "Missing LEN in Type");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != ASSOP){
		ParseError(line, "Missing ASSOP in Type");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != ICONST){
		ParseError(line, "Missing ICONST in Type");
		return false;
	}
	defSuccessMessages.push("Definition of Strings with length of "+t.GetLexeme()+" in declaration statement.");
	int strlen = stoi(t.GetLexeme());
	Value strlenVal = Value(strlen);
	TempsResults["strlen"] = strlenVal; //set up with key strlen
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN){
		ParseError(line, "Missing RPAREN in Type");
		return false;
	}
	return true;

}// End of Type


//VarList:= Var [= Expr] {, Var [= Expr]}
bool VarList(istream& in, int& line, LexItem & idtok, int strlen) { //default value? strlen=1?
	LexItem t;

	if(!Var(in,line,idtok)){
		ParseError(line, "Missing Var in VarList");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	string varName = t.GetLexeme();
	if(!(defVar.find(t.GetLexeme()) == defVar.end())){
		ParseError(line, "Var already defined");
		return false;
	}
	defVar[varName] = true;
	SymTable[varName] = idtok.GetToken(); 

	t = Parser::GetNextToken(in, line);

	if(t == ASSOP){
		assignVar[varName] = true;
		//t = Parser::GetNextToken(in, line);
		Value ExprReturn;
		if(!Expr(in, line, ExprReturn)){
			ParseError(line, "Missing Expr in VarList");
			return false;
		}
		if(SymTable[varName] == INTEGER){
			if(!ExprReturn.IsInt()){
				ParseError(line, "Setting non int to type int");
				return false;
			}
		}
		if(SymTable[varName] == REAL){
			if(ExprReturn.IsInt()){
				double newReturn = ExprReturn.GetInt();
				ExprReturn = Value(newReturn);
			} else if(!ExprReturn.IsReal()){
				ParseError(line, "Setting non real to type real");
				return false;
			}
		}
		if(SymTable[varName] == CHARACTER){
			if(!ExprReturn.IsString()){
				ParseError(line, "Setting non char to type char");
				return false;
			}
			ExprReturn.SetstrLen(strlen);
			if((ExprReturn.GetString()).length() != ExprReturn.GetstrLen()){
				if((ExprReturn.GetString()).length() > ExprReturn.GetstrLen()){
					//str is greater than str length (substring)
					ExprReturn.SetString(ExprReturn.GetString().substr(0,ExprReturn.GetstrLen()));
				} else {
					//str is less than str length (pad)
					string tempStr = ExprReturn.GetString();
					for(int i = 0; i < (ExprReturn.GetstrLen() - ExprReturn.GetString().length()); ++i){
						tempStr += " ";
					}
					ExprReturn.SetString(tempStr);
				}
			}
		}
		TempsResults[varName]= ExprReturn;
		initSuccessMessages.push(initialized);
		t = Parser::GetNextToken(in, line);
	} else {
		Value temp;
		if(SymTable[varName] == INTEGER){
			temp = Value(0);
		}
		if(SymTable[varName] == REAL){
			double tempD = 0;
			temp = Value(tempD);
		}
		if(SymTable[varName] == CHARACTER){
			string tempS = "";
			for (int i=0;i<strlen;++i){
				tempS+=" ";
			}
			temp = Value(tempS);
			temp.SetstrLen(strlen);
			assignVar[varName]=true;//always true? or only true when len specified?
		}
		TempsResults[varName]= temp;
		initSuccessMessages.push(initialized);
	}

	
	if (t == COMMA) {
		return VarList(in, line, idtok, strlen);
	}
	else if(t.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(t);
		return true;
	}

}// End of VarList


//Stmt:= AssignStmt | BlockIfStmt | PrintStmt | SimpleIfStmt (Changed to AssignStmt | BlockIfStmt | PrintStmt)
bool Stmt(istream& in, int& line) {
	LexItem t;
	t = Parser::GetNextToken(in, line);

	if(t==IDENT){
		Parser::PushBackToken(t);
		if(!AssignStmt(in,line)){
			ParseError(line, "Assign Stmt Syntax error");
			return false;
		}
		return true;
	}

	if(t==PRINT){
		Parser::PushBackToken(t);
		if(!PrintStmt(in,line)){
			ParseError(line, "Print Stmt Syntax error");
			return false;
		}
		return true;
	}

	if(t==IF){
		Parser::PushBackToken(t);
		if(!BlockIfStmt(in,line)){
			ParseError(line, "If Stmt Syntax error");
			return false;
		}
		return true;
	}

	ParseError(line, "Stmt Syntax error");
	return false;
}// End of Stmt


//SimpleStmt:= AssigNStmt | PrintStmt
bool SimpleStmt(istream& in, int& line) {
	LexItem t;
	t = Parser::GetNextToken(in, line);

	if(t==IDENT){
		Parser::PushBackToken(t);
		if(!AssignStmt(in,line)){
			ParseError(line, "Assign Stmt Syntax error");
			return false;
		}
		ifSuccessMessages.push("Assign statement in a Simple If statement.");
		return true;
	}

	if(t==PRINT){
		Parser::PushBackToken(t);
		if(!PrintStmt(in,line)){
			ParseError(line, "Print Stmt Syntax error");
			return false;
		}
		ifSuccessMessages.push("Print statement in a Simple If statement.");
		return true;
	}
	ParseError(line, "SimpleStmt Syntax error");
	return false;
}// End of SimpleStmt


//BlockIfStmt:= IF (RelExpr) THEN {Stmt} [ELSE {Stmt}] END IF (CHANGED TO IF (RelExpr) ( SimpleIfStmt | THEN {Stmt} [ELSE {Stmt}] END IF) )
bool BlockIfStmt(istream& in, int& line) {
	LexItem t;
	t = Parser::GetNextToken(in, line);
	if(t!=IF){
		ParseError(line, "Missing IF in BlockIfStmt");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t!=LPAREN){
		ParseError(line, "Missing LPAREN in BlockIfStmt");
		return false;
	}
	
	Value relExprReturn;
	if(!RelExpr(in, line, relExprReturn)){
		ParseError(line, "Missing RelExpr in BlockIfStmt");
		return false;
	}
	if(!relExprReturn.GetBool()){
		runCode = false;
	}
	t = Parser::GetNextToken(in, line);
	if(t!=RPAREN){
		ParseError(line, "Missing RPAREN in BlockIfStmt");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	nestLevel++;
	if(t!=THEN){
		Parser::PushBackToken(t);
		if(!SimpleIfStmt(in,line)){
			ParseError(line, "Missing RelExpr in BlockIfStmt");
			return false;
		}
		runCode = true;
		nestLevel--;
		return true;
	}
	while(!runCode){
		t=Parser::GetNextToken(in, line);
		if(t==END || t==ELSE){
			Parser::PushBackToken(t);
			break;
		}
	}
	while(runCode){
		if(Stmt(in, line)){
			t = Parser::GetNextToken(in, line);
			if(t==END || t==ELSE){
				Parser::PushBackToken(t);
				break;
			}
			Parser::PushBackToken(t);
		} else {
			ParseError(line, "Missing Stmt in BlockIfStmt");
			return false;
		}
	}
	t = Parser::GetNextToken(in, line);
	if(t==ELSE){
		runCode = !runCode;
		while(!runCode){
			t=Parser::GetNextToken(in, line);
			if(t==END){
				Parser::PushBackToken(t);
				break;
			}
		}
		while(runCode){
			if(Stmt(in, line)){
				t = Parser::GetNextToken(in, line);
				if(t==END){
					Parser::PushBackToken(t);
					break;
				}
				Parser::PushBackToken(t);
			} else {
				ParseError(line, "Missing Stmt in BlockIfStmt");
				return false;
			}
		}
		t = Parser::GetNextToken(in, line);
	}
	if(t!=END){
		ParseError(line, "Missing END in BlockIfStmt");
		return false;
	}
	runCode = true;
	ifSuccessMessages.push("End of Block If statement at nesting level "+to_string(nestLevel));
	nestLevel--;
	t = Parser::GetNextToken(in, line);
	if(t!=IF){
		ParseError(line, "Missing IF in BlockIfStmt");
		return false;
	}
	return true;

}// End of BlockIfStmt


//SimpleIfStmt:= IF (RelExpr) SimpleStmt (changed to SimpleStmt)
bool SimpleIfStmt(istream& in, int& line) {
	if(!SimpleStmt(in, line)){
		ParseError(line, "Missing SimpleStmt in SimpleIfStmt");
		return false;
	}
	return true;
}// End of SimpleIfStmt


//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	LexItem t;
	LexItem idtok; //get from SymTable??
	if(!Var(in, line, idtok)){
		ParseError(line, "Missing Var in AssignStmt");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	
	if(defVar.find(t.GetLexeme()) == defVar.end()){
		ParseError(line, "Var not defined");
		return false;
	}
	string varName = t.GetLexeme();
	Value oldVar = TempsResults[varName];
	int strlen;
	if(oldVar.IsString()){
		strlen = oldVar.GetstrLen();
	}
	t = Parser::GetNextToken(in, line);
	if(t!=ASSOP){
		ParseError(line, "Missing ASSOP in AssignStmt");
		return false;
	}
	Value ExprReturn;
	if(!Expr(in, line, ExprReturn)){
		ParseError(line, "Missing Expr in AssignStmt");
		return false;
	}
	if(runCode){
		assignVar[varName]=true;
	}
	if((SymTable[varName] == INTEGER)&&runCode){
			if(!ExprReturn.IsInt()){
				line = t.GetLinenum();
				ParseError(line, "Setting non int to type int");
				return false;
			}
		}
		if((SymTable[varName] == REAL)&&runCode){
			if(ExprReturn.IsInt()){
				double newReturn = ExprReturn.GetInt();
				ExprReturn = Value(newReturn);
			} else if(!ExprReturn.IsReal()){
				line = t.GetLinenum();
				ParseError(line, "Setting non real to type real");
				return false;
			}
		}
		if((SymTable[varName] == CHARACTER)&&runCode){
			if(!ExprReturn.IsString()){
				line = t.GetLinenum();
				ParseError(line, "Setting non char to type char");
				return false;
			}
			ExprReturn.SetstrLen(strlen);
			if((ExprReturn.GetString()).length() != ExprReturn.GetstrLen()){
				if((ExprReturn.GetString()).length() > ExprReturn.GetstrLen()){
					//str is greater than str length (substring)
					ExprReturn.SetString(ExprReturn.GetString().substr(0,ExprReturn.GetstrLen()));
				} else {
					//str is less than str length (pad)
					string tempStr = ExprReturn.GetString();
					for(int i = 0; i < (ExprReturn.GetstrLen() - ExprReturn.GetString().length()); ++i){
						tempStr += " ";
					}
					ExprReturn.SetString(tempStr);
				}
			}
	}
	if(runCode){
		TempsResults[varName]= ExprReturn;
	}
	return true;
}// End of AssignStmt


//Var:= IDENT
bool Var(istream& in, int& line, LexItem & idtok) {
	LexItem t;
	t = Parser::GetNextToken(in, line);
	if(t!=IDENT){
		ParseError(line, "Missing IDENT in Var");
		Parser::PushBackToken(t);
		return false;
	}
	initialized = "Initialization of the variable "+t.GetLexeme()+" in the declaration statement.";
	Parser::PushBackToken(t);
	return true;
}// End of Var


//RelExpr:= Expr [ ( == | < | > ) Expr ]
bool RelExpr(istream& in, int& line, Value & retVal) {
	LexItem t;
	Value exprReturn1;
	if(!Expr(in, line, exprReturn1)){
		ParseError(line, "Missing Expr in RelExpr");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t!=GTHAN && t!=LTHAN && t!= EQ){
		Parser::PushBackToken(t);
		retVal = exprReturn1;
		if(!retVal.IsBool()){
			line = t.GetLinenum();
			ParseError(line, "Run-Time Error-Illegal Type for If statement condition");
			return false;
		}
		return true;
	} else {
		Value exprReturn2;
		LexItem relation = t;
		if(!Expr(in, line, exprReturn2)){
			ParseError(line, "Missing Expr in RelExpr");
			return false;
		}
		if(relation==GTHAN){
			retVal = exprReturn1 > exprReturn2;
		} else if(relation==LTHAN){
			retVal = exprReturn1 < exprReturn2;
		} else if(relation==EQ){
			retVal = exprReturn1 == exprReturn2;
		}
	}
	if(retVal.IsErr()){
		line = t.GetLinenum();
		ParseError(line, "Illegal Use of Relation Operators");
		return false;
	}
	if(!retVal.IsBool()){
		line = t.GetLinenum();
		ParseError(line, "Run-Time Error-Illegal Type for If statement condition");
		return false;
	}
	return true;
}// End of RelExpr


//Expr:= MultExpr { ( + | - | // ) MultExpr }
bool Expr(istream& in, int& line, Value & retVal) {
	LexItem t;
	Value multExprReturn;
	if(!MultExpr(in, line, multExprReturn)){
		ParseError(line, "Missing MultExpr in Expr");
		return false;
	}
	retVal=multExprReturn;
	t = Parser::GetNextToken(in, line);
	if(t==PLUS){
		Value exprReturn;
		if(!Expr(in, line, exprReturn)){
			ParseError(line, "Missing Expr in Expr");
			return false;
		}
		retVal = retVal + exprReturn;
		if(retVal.IsErr()){
			line = t.GetLinenum();
			ParseError(line, "Illegal Use of Plus Operator");
			return false;
		}
		return true;
	} else if (t==MINUS){
		Value exprReturn;
		if(!Expr(in, line, exprReturn)){
			ParseError(line, "Missing Expr in Expr");
			return false;
		}
		retVal = retVal - exprReturn;
		if(retVal.IsErr()){
			line = t.GetLinenum();
			ParseError(line, "Illegal Use of Minus Operator");
			return false;
		}
		return true;
	} else if (t==CAT){
		Value exprReturn;
		if(!Expr(in, line, exprReturn)){
			ParseError(line, "Missing Expr in Expr");
			return false;
		}
		retVal = retVal.Catenate(exprReturn);
		if(retVal.IsErr()){
			line = t.GetLinenum();
			ParseError(line, "Illegal Use of Catenate Operator");
			return false;
		}
		return true;
	} else {
		Parser::PushBackToken(t);
		return true;
	}
	// if(t!=PLUS && t!=MINUS && t!= CAT){
	// 	Parser::PushBackToken(t);
	// 	return true;
	// } else {
	// 	return Expr(in, line);
	// }
	// return true; //OLD CODE
}// End of Expr

//MultExpr:= TermExpr { ( * | / ) TermExpr }
bool MultExpr(istream& in, int& line, Value & retVal) {
	LexItem t;
	Value termExprReturn;
	if(!TermExpr(in, line, termExprReturn)){
		ParseError(line, "Missing TermExpr in MultExpr");
		return false;
	}
	retVal=termExprReturn;
	t = Parser::GetNextToken(in, line);
	if(t==MULT){
		Value multExprReturn;
		if(!MultExpr(in, line, multExprReturn)){
			ParseError(line, "Missing MultExpr in MultExpr");
			return false;
		}
		retVal = retVal * multExprReturn;
		if(retVal.IsErr()){
			line = t.GetLinenum();
			ParseError(line, "Illegal Use of Mult Operator");
			return false;
		}
		return true;
	} else if (t==DIV){ 
		Value multExprReturn;
		if(!MultExpr(in, line, multExprReturn)){
			ParseError(line, "Missing MultExpr in MultExpr");
			return false;
		}
		retVal = retVal/multExprReturn;
		if(retVal.IsErr()){
			line = t.GetLinenum();
			ParseError(line, "Illegal Use of Divider Operator");
			return false;
		}
		if(multExprReturn.IsInt()&&multExprReturn.GetInt()==0){
			line = t.GetLinenum();
			ParseError(line, "Run-Time Error-Illegal division by Zero");
			return false;
		}
		if(multExprReturn.IsReal()&&multExprReturn.GetReal()==0){
			line = t.GetLinenum();
			ParseError(line, "Run-Time Error-Illegal division by Zero");
			return false;
		}
		return true;
	} else {
		Parser::PushBackToken(t);
		return true;
	}
	// if(t!=MULT && t!=DIV){
	// 	Parser::PushBackToken(t);
	// 	return true;
	// } else {
	// 	return MultExpr(in, line);
	// } //OLD CODE
}// End of MultExpr

//TermExpr:= SFactor { ** SFactor }
bool TermExpr(istream& in, int& line, Value & retVal) {
	LexItem t;
	Value sFactorReturn;
	if(!SFactor(in, line, sFactorReturn)){
		ParseError(line, "Missing SFactor in TermExpr");
		return false;
	}
	retVal = sFactorReturn;
	t = Parser::GetNextToken(in, line);
	if(t!=POW){
		Parser::PushBackToken(t);
		return true;
	}
	Value termExprReturn;
	if(!TermExpr(in, line, termExprReturn)){
		ParseError(line, "Missing TermExpr in TermExpr");
		return false;
	}
	retVal = retVal.Power(termExprReturn);
	if(retVal.IsErr()){
		line = t.GetLinenum();
		ParseError(line, "Illegal Use of Power Operator");
		return false;
	}
	return true;
	
}// End of TermExpr


//SFactor:= [+ | -] Factor
bool SFactor(istream& in, int& line, Value & retVal) {
	LexItem t;
	int sign = 1;
	t = Parser::GetNextToken(in, line);
	if(t==MINUS){
		sign = -1;
	}
	bool signedFlag = false;
	if(t!=PLUS && t!=MINUS){
		Parser::PushBackToken(t);
	} else {
		signedFlag = true;
	}
	if(!Factor(in, line, sign, retVal)){
		ParseError(line, "Missing Factor in SFactor");
		return false;
	}
	if(retVal.IsString() && signedFlag){ 
		ParseError(line, "Illegal Operand Type for Sign Operator");
		return false;
	}

	return true;
}// End of SFactor


//Factor:= IDENT | ICONST | RCONST | SCONST | (Expr)
bool Factor(istream& in, int& line, int sign, Value & retVal) {
	LexItem t;
	t = Parser::GetNextToken(in, line);
	if (t == ICONST){
		Value signVal = Value(sign);
		retVal = signVal * Value(stoi(t.GetLexeme()));
		return true;
	}
	if (t == RCONST){
		Value signVal = Value(sign);
		retVal = signVal * Value(stod(t.GetLexeme()));
		return true;
	}
	if (t == SCONST){
		if(sign==-1){
			ParseError(line, "String cannot be signed");
			return false;
		}
		retVal = Value(t.GetLexeme());
		return true;
	}
	if (t == IDENT){
		if(defVar.find(t.GetLexeme()) == defVar.end()){
			ParseError(line, "Var not defined");
			return false;
		}
		if(assignVar.find(t.GetLexeme()) == assignVar.end()){
			ParseError(line, "Var not assigned to value");
			return false;
		}
		Value SFactorform = TempsResults[t.GetLexeme()];
		if(sign == -1){
			Value neg = Value(sign);
			retVal = neg * SFactorform;
			if(retVal.IsErr()){
				ParseError(line, "Illegal Operand Type for Sign Operator");
				return false;
			}
		} else {
			retVal = SFactorform;
			if(retVal.IsErr()){
				ParseError(line, "Illegal SFactor");
				return false;
			}
		}
		return true;
	}
	if(t!=LPAREN){
		ParseError(line, "Missing LPAREN in Factor");
		return false;
	}
	Value ExprReturn;
	if(!Expr(in, line, ExprReturn)){
		ParseError(line, "Missing Expr in Factor");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t!=RPAREN){
		ParseError(line, "Missing RPAREN in Factor");
		return false;
	}
	if(sign == -1){
		Value neg = Value(sign);
		retVal = neg * ExprReturn;
		if(retVal.IsErr()){
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
	} else {
		retVal = ExprReturn;
		if(retVal.IsErr()){
			ParseError(line, "Illegal Expresion in parenthesis");
			return false;
		}
	}

	return true;
}// End of Factor
