%{
	#include <iostream>
	#include <vector>
	#include <string>
	#include <map>
	#include <vector>
	#include "c.hpp"
	#include "codes.hpp"

 	using namespace std;

 	extern int yylineno;
 	extern char *yytext;
	extern int yylex();

 	union utype
	{
		Node *node;
		char *s;
	};
	#define YYSTYPE utype
	#define YYDEBUG 1

	void yyerror (char const *s) {
   		cerr << s << endl;
 	}
%}

%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN 
%token RIGHT_OP LEFT_OP INC_OP DEC_OP PTR_OP AND_OP OR_OP LE_OP GE_OP EQ_OP NE_OP 
%token IDEN DATA_TYPE STRUCT MODIFIER
%token IF ELSE WHILE FOR DO 
%token CONTINUE BREAK RETURN 
%token FLOATING_POINT INTEGER CHAR_LIT STRING VOID
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
 
%%


strt:  					external_declaration strt 
								{
									root = $1.node;
									root->code = STRT;
									log(string(yytext) + " strt:external_declaration strt", printFlag);
								}
						| /* empty */
						;

external_declaration: 	VOID 
						identifier_E
						left_parenthesis_E 
								{ 
									currentFunction = $2.node->name;
									scopepp();
								} 
						arg_list 
								{ 
									scope--;
								} 
						right_parenthesis
						compound_stmts_E 			
								{
									vector<Node *> rhs = {$1.node, $4.node, $4.node, $4.node, $5.node, $6.node};
									$$.node = new Node(EXTERNAL_DECLARATION, rhs);
									Function function = Function($2.node->name, t_void, variableTables[scope].getVariableNames());
									functionTable.push_back(function);
									log(string(yytext) + " external_declaration:VOID identifier_E left_parenthesis_E arg_list right_parenthesis compound_stmts_E", printFlag);
								}
						| data_type_non_struct_E identifier_E external_non_struct_LR			
								{
									vector<Node *> rhs = {$1.node, $2.node, $3.node};
									$$.node = new Node(EXTERNAL_DECLARATION, rhs);
									cerr << "Type = " << $1.node->type << "Name = " << $2.node->name;
									if($3.node->type == t_func){
										Function function = Function($2.node->name, $1.node->type, variableTables[scope].getVariableNames());
										functionTable.push_back(function);
									}
									else{
										Variable variable = Variable($2.node->name, $1.node->type, {}); // CHECK IF variable is arrray and set dimlist
										variableTables[scope].rows.push_back(variable);
										//Check with ($3.node->rhs)[0].type
										cout <<"Kya be" << ($3.node->rhs)[0]->type << endl;
										if((($3.node->rhs)[0]->type != "") && (!areCoercibleType($1.node->type, ($3.node->rhs)[0]->type))){
											cerr << "Semantic error: Assigning " + $1.node->type + " to variable of type" + ($3.node->rhs)[0]->type + " in line no. " << yylineno << endl;
										}
										Node *temp = ($2.node->rhs)[1];
										if ((temp->rhs).size() != 0)
										{	
											currentFunction = $2.node->name;
											temp = (temp->rhs)[1];
											vector<Node *> nodes = getNodeList(temp, 0, 1, 1);
											for (auto currentNode:nodes)
											{
												if((currentNode->type != "") && (!areCoercibleType($1.node->type, (currentNode->type)))){
													semanticError = true;
													cerr << "Semantic error: Assigning " + $1.node->type + " to variable of type" + currentNode->type + " in line no. " << yylineno << endl;
												}

												currentNode->type = $1.node->type;
												Variable variable = Variable(currentNode->name, currentNode->type, {}); // CHECK IF variable is arrray and set dimlist
												variableTables[scope].rows.push_back(variable);
											}
										}
									}
									log(string(yytext) + " external_declaration:data_type_non_struct_E identifier_E external_non_struct_LR", printFlag);
								}
						| STRUCT identifier_E external_struct_LR1                     
								{
									vector<Node *> rhs = {$1.node, $2.node, $3.node};
									$$.node = new Node(EXTERNAL_DECLARATION, rhs);
									vector<Node *> node_list = getNodeList($3.node->rhs[1], 0, 2, 0); // MODIFY THIS
									vector<string> attribute_names, attribute_types;
									for(int i=0;i<node_list.size();i++){
										attribute_types[i] = node_list[i]->rhs[0]->type;
										attribute_names[i] = node_list[i]->rhs[0]->name;
									}
									Struct str = Struct($2.node->name, attribute_names, attribute_types, {});
									structTables[scope].rows.push_back(str);
									log(string(yytext) + " external_declaration:STRUCT identifier_E external_struct_LR1", printFlag);
								}
						;

external_non_struct_LR:	left_parenthesis
								{
									scopepp();
									currentFunction = $$.node->name;
								}
						arg_list
								{
									scopemm();
								}
						right_parenthesis
						compound_stmts_E			
								{
									vector<Node *> rhs = {$1.node, $2.node, $3.node, $4.node};
									$$.node = new Node(EXTERNAL_NON_STRUCT_LR, rhs);
									$$.node->type = t_func;
									log(string(yytext) + " external_non_struct_LR:	left_parenthesis arg_list right_parenthesis compound_stmts_E", printFlag);
								}
						| optional_assignment optional_assigned_identifier_list_ext semi            
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(EXTERNAL_NON_STRUCT_LR, rhs);
									log(string(yytext) + " external_non_struct_LR:	optional_assignment optional_assigned_identifier_list_ext", printFlag);
								}
						
						;

external_struct_LR1: 	identifier external_struct_LR2	//Function returning struct or Object declaration			
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(EXTERNAL_STRUCT_LR1, rhs);
									// $$.node->type = string(isStruct($1.node->type))
									log(string(yytext) + " external_struct_LR1:identifier external_struct_LR2", printFlag);
								}
						| left_brace declaration_list right_brace semi 	// Struct definition
						/* HANDLE SYNTAX ERROR */                     
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(EXTERNAL_STRUCT_LR1, rhs);
									log(string(yytext) + " external_struct_LR1:left_brace declaration_list right_brace semi", printFlag);
								}
						;

external_struct_LR2:	left_parenthesis { scopepp(); } arg_list { scopemm(); } right_parenthesis compound_stmts_E //Function returning struct
						/* HANDLE SYNTAX ERROR */                     
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node, $4.node };
									$$.node = new Node(EXTERNAL_STRUCT_LR2, rhs);
									log(string(yytext) + " external_struct_LR2:optional_identifier_list_ext semi", printFlag);
								}
						| ',' identifier_list_E semi
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(EXTERNAL_STRUCT_LR2, rhs);
									log(string(yytext) + " external_struct_LR2: , identifier_list_E semi", printFlag);
								}
						| ';'
								{
									vector<Node *> rhs = {};
									$$.node = new Node(EXTERNAL_STRUCT_LR2, rhs);
									log(string(yytext) + " external_struct_LR2: ;", printFlag);
								}
						| error 
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(EXTERNAL_STRUCT_LR2, rhs);
									cerr << "Missing semicolon or comma at line no. " << yylineno;
								}
						;

stmt_list:				stmt stmt_list			
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(STMT_LIST, rhs);
									log(string(yytext) + " stmt_list: stmt stmt_list", printFlag);
								}
						|/* empty */
						;

stmt:					declaration_non_struct semi			
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(STMT, rhs);
									log(string(yytext) + "stmt: declaration_non_struct semi", printFlag);
								}
						| conditional_stmt			
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(STMT, rhs);
									log(string(yytext) + "stmt: conditional_stmt", printFlag);
								}
						| iteration_stmt			
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(STMT, rhs);
									log(string(yytext) + "stmt: iteration_stmt", printFlag);
								}
						| jump_stmt			
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(STMT, rhs);
									log(string(yytext) + "stmt: jump_stmt", printFlag);
								}
						| expr semi			
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(STMT, rhs);
									log(string(yytext) + "stmt: expr semi", printFlag);
								}
						| struct_stmt   //Struct definition or object declaration
						/* HANDLE SYNTAX ERROR */                     
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(STMT, rhs);
									log(string(yytext) + "stmt: struct_stmt ", printFlag);
								}
						;

struct_stmt:			STRUCT identifier_E struct_stmt_LR                     
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node };
									$$.node = new Node(STRUCT_STMT, rhs);
									log(string(yytext) + "struct_stmt: STRUCT identifier_E struct_stmt_LR", printFlag);
								}
						;	
	
struct_stmt_LR:			identifier_list semi // Object declaration			
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(STRUCT_STMT_LR, rhs);
									log(string(yytext) + "struct_stmt_LR :identifier_list semi ", printFlag);
								}
						| left_brace declaration_list right_brace semi // Struct definition
						/* HANDLE SYNTAX ERROR */                     
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(STRUCT_STMT_LR, rhs);
									log(string(yytext) + "struct_stmt_LR: left_brace declaration_list right_brace semi ", printFlag);
								}
						;

compound_stmts:			left_brace
						stmt_list
								{
   									allVariableTables.push_back(make_pair(variableTables[variableTables.size()-1], currentFunction));
								}
						right_brace                     
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(COMPOUND_STMTS, rhs);
									log(string(yytext) + "compound_stmts: left_brace stmt_list right_brace ", printFlag);
								}
						;

compound_stmts_E:		left_brace_E stmt_list right_brace                     
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node };
									$$.node = new Node(COMPOUND_STMTS_E, rhs);
									log(string(yytext) + "compound_stmts_E: left_brace_E stmt_list right_brace  ", printFlag);
								}
						;

optional_compound_stmt: compound_stmts						 
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(OPTIONAL_COMPOUND_STMT, rhs);
									log(string(yytext) + "optional_compound_stmt: compound_stmts", printFlag);
								}
						| stmt
						/* HANDLE SYNTAX ERROR */ 
						;

declaration_list: 		declaration semi optional_decl_ext       
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node };
									$$.node = new Node(DECLARATION_LIST, rhs);
									log(string(yytext) + "declaration_list: declaration semi optional_decl_ext", printFlag);
								}
						;

optional_decl_ext:		declaration_list				 
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(OPTIONAL_DECL_EXT, rhs);
									log(string(yytext) + "optional_decl_ext:declaration_list", printFlag);
								}
						| /* empty */
						;

declaration:			declaration_non_struct                 
								{
									vector<Node *> rhs = {$1.node};
									$$.node = new Node(DECLARATION, rhs);
									$$.node->type = $1.node->type;
									log(string(yytext) + " declaration:declaration_non_struct_E ", printFlag);
								}
					    | declaration_struct                     
								{
									vector<Node *> rhs = {$1.node};
									$$.node = new Node(DECLARATION, rhs);
									$$.node->type = $1.node->type;

									log(string(yytext) + " declaration:declaration_struct", printFlag);
								}
						;

declaration_non_struct: data_type_non_struct assigned_identifier_list              
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(DECLARATION_NON_STRUCT, rhs);
									log(string(yytext) + " declaration_non_struct: data_type_non_struct assigned_identifier_list ", printFlag);

									// tempNode is of ASSIGNED_IDENTIFIER_LIST
									Node *tempNode = $2.node;
									$$.node->type = $1.node->type;

									vector<Node *> nodes = getNodeList($2.node, 0, 1, 1);

									for(auto node1:nodes){
											// currentNode is of ASSIGNED_IDENTIFIER
										if((node1->type != "") && !areCoercibleType($1.node->type, node1->type)){
											semanticError = true;
											cout << "Syntax error: Assigning type " + node1->type + " to data type " + $1.node->type + " in line no. " << yylineno << endl;
										}
										node1->type = $1.node->type; // patch type
										addToVariableTable(node1);
									}
								}
						;

declaration_struct:		STRUCT identifier_E identifier_list_E                     
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node };
									$$.node = new Node(DECLARATION_STRUCT, rhs);
									log(string(yytext) + " declaration_struct: STRUCT identifier_E identifier_list_E ", printFlag);
								
									// tempNode is of IDENTIFIER_LIST_E
									Node *tempNode = $3.node;
									$$.node->type = $1.node->name;


									vector<Node *> nodes = getNodeList($3.node, 0, 1, 1);

									for(auto node:nodes){
											// currentNode is of ASSIGNED_IDENTIFIER
										node->type = $1.node->name; // patch type
										addToVariableTable(node);
									}

								}
						;

assigned_identifier_list: assigned_identifier optional_assigned_identifier_list_ext  
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(ASSIGNED_IDENTIFIER_LIST, rhs);
									log(string(yytext) + " assigned_identifier_list: assigned_identifier optional_assigned_identifier_list_ext", printFlag);
								}
						;

optional_assigned_identifier_list_ext: ',' assigned_identifier_list			
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(OPTIONAL_ASSIGNED_IDENTIFIER_LIST_EXT, rhs);
									log(string(yytext) + " optional_assigned_identifier_list_ext: , assigned_identifier_list", printFlag);
								}
						| /* empty */
								{
									vector<Node *> rhs = {};
									$$.node = new Node(OPTIONAL_ASSIGNED_IDENTIFIER_LIST_EXT, rhs);
									log(string(yytext) + " optional_assigned_identifier_list_ext: empty", printFlag);
								}
						;

identifier_list: 		identifier optional_identifier_list_ext                     
								{
									vector<Node *> rhs = {$1.node, $2.node };
									$$.node = new Node(IDENTIFIER_LIST, rhs);
									log(string(yytext) + " identifier_list:identifier optional_identifier_list_ext", printFlag);
								}
						;

identifier_list_E: 		identifier_E optional_identifier_list_ext                     
								{
									vector<Node *> rhs = { $1.node, $2.node};
									$$.node = new Node(IDENTIFIER_LIST_E, rhs);
									log(string(yytext) + " identifier_list_E:identifier_E optional_identifier_list_ext", printFlag);
								}
						;

optional_identifier_list_ext: ',' identifier_list_E			
								{
									vector<Node *> rhs = { $1.node, $2.node};
									$$.node = new Node(OPTIONAL_IDENTIFIER_LIST_EXT, rhs);
									log(string(yytext) + " optional_identifier_list_ext:comma identifier_list_E", printFlag);
								}
						| /* empty */
						;

assigned_identifier: 	identifier_E optional_assignment        
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(ASSIGNED_IDENTIFIER, rhs);
									$$.node->name = $1.node->name;
									if($2.node->value != ""){
										$$.node->value = $2.node->value;
									}
									$$.node->dimlist = $1.node->dimlist;
									$$.node->type = $2.node->type;
									cout << ($1.node->dimlist).size() << endl;
										log("DEBUG :" + $1.node->type + " " + $2.node->type , printFlag);
									log(string(yytext) + " assigned_identifier:identifier_E optional_assignment", printFlag);
								}
						;

optional_assignment:	assignment_op expr_E			
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(OPTIONAL_ASSIGNMENT, rhs);
									$$.node->value = $2.node->value;
									$$.node->type = $2.node->type;
									log(string(yytext) + " optional_assignment:	assignment_op expr_E", printFlag);
								}
						| /* empty */
								{
									vector<Node *> rhs = {};
									$$.node = new Node(OPTIONAL_ASSIGNMENT, rhs);
									$$.node->type = "";
									log(string(yytext) + " optional_assignment:empty", printFlag);
								}
						;

arg_list:				data_type_non_struct identifier_E optional_arg_list_extension	
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node };
									$$.node = new Node(ARG_LIST, rhs);
									$2.node->type = $1.node->type;
									log(string(yytext) + " arg_list:data_type identifier_E optional_arg_list_extension", printFlag);
								}
						| STRUCT IDEN identifier_E optional_arg_list_extension	
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node, $4.node };
									$$.node = new Node(ARG_LIST, rhs);
									log(string(yytext) + " arg_list:data_type identifier_E optional_arg_list_extension", printFlag);
								}
						| /* empty */
						;

optional_arg_list_extension: ',' arg_list			
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(OPTIONAL_ARG_LIST_EXTENSION, rhs);
									log(string(yytext) + " optional_arg_list_extension: ',' arg_list", printFlag);
								}
						| /* empty */
						;

param_list:				expr optional_param_extension			
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(PARAM_LIST, rhs);
									log(string(yytext) + "param_list: expr_E optional_param_extension", printFlag);
								}
						| /* empty */ 
						;

optional_param_extension: ',' param_list			
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(OPTIONAL_PARAM_EXTENSION, rhs);
									log(string(yytext) + "optional_param_extension: ',' param_list", printFlag);
								}
						| /* empty */
						;

conditional_stmt:		IF left_parenthesis_E expr_E right_parenthesis optional_compound_stmt optional_else  
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node, $4.node, $5.node, $6.node };
									$$.node = new Node(CONDITIONAL_STMT, rhs);
									log(string(yytext) + " conditional_stmt: IF left_parenthesis_E expr_E right_parenthesis optional_compound_stmt optional_else ", printFlag);
								}
						;

optional_else:			ELSE optional_compound_stmt			
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(OPTIONAL_ELSE, rhs);
									log(string(yytext) + "optional_else: ELSE optional_compound_stmt	", printFlag);
								}
						| /* empty */ %prec LOWER_THAN_ELSE
						;

iteration_stmt:			WHILE left_parenthesis_E expr_E right_parenthesis optional_compound_stmt 			
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node, $4.node, $5.node };
									$$.node = new Node(ITERATION_STMT, rhs);
									log(string(yytext) + "iteration_stmt: WHILE left_parenthesis_E expr_E right_parenthesis optional_compound_stmt ", printFlag);
								}
						| DO optional_compound_stmt WHILE left_parenthesis_E expr_E right_parenthesis semi			
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node, $4.node, $5.node, $6.node, $7.node };
									$$.node = new Node(ITERATION_STMT, rhs);
									log(string(yytext) + "iteration_stmt: DO optional_compound_stmt WHILE left_parenthesis_E expr_E right_parenthesis semi", printFlag);
								}
						| FOR left_parenthesis_E optional_expr semi expr_E semi optional_expr right_parenthesis optional_compound_stmt 
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node, $4.node, $5.node, $6.node, $7.node, $8.node };
									$$.node = new Node(ITERATION_STMT, rhs);
									log(string(yytext) + "iteration_stmt: FOR left_parenthesis_E expr_E semi expr_E semi expr_E right_parenthesis optional_compound_stmt ", printFlag);
								}
						;

jump_stmt:				CONTINUE semi				
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(JUMP_STMT, rhs);
									log(string(yytext) + " jump_stmt:CONTINUE semi", printFlag);
								}
						| BREAK semi				
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(JUMP_STMT, rhs);
									log(string(yytext) + " jump_stmt:BREAK sem", printFlag);
								}
						| RETURN optional_expr semi 
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node};
									$$.node = new Node(JUMP_STMT, rhs);
									if(functionTable[functionTable.size()-1].rtype != $2.node->type){
										cerr << "Semantic Error: Return value doesn't match return type" << endl;
									}
									log(string(yytext) + " jump_stmt:RETURN optional_expr semi", printFlag);
								}
						;

optional_expr:			expr						
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(OPTIONAL_EXPR, rhs);
									$$.node->type = $1.node->type;
									log(string(yytext) + " optional_expr:expr_E", printFlag);
								}
						| /* empty */
							{
									vector<Node *> rhs = {};
									$$.node = new Node(OPTIONAL_EXPR, rhs);
									log(string(yytext) + " OPTIONAL_EXPR:empty", printFlag);
							}
						;

expr:					number expr_LR1				
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(EXPR, rhs);
									//TODO Check if number and expression are type coercible
									if (($2.node->rhs).size() != 0)
									{
										$$.node->value = applyOperation($1.node , ($2.node->rhs)[0], ($2.node->rhs)[1]);
									} else $$.node->value = $1.node->value;
									//TODO: Define applyOperation
									$$.node->type = $1.node->type;
									log(string(yytext) + " expr:number expr_LR1	", printFlag);
								}
						| identifier expr_LR2       
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(EXPR, rhs);
									$$.node->type = variableTables[scope].getVariableType($1.node->name);
									log(string(yytext) + " expr:identifier expr_LR2", printFlag);
								}
						| left_parenthesis expr_E right_parenthesis
								{
									vector<Node *> rhs = {$1.node, $2.node, $3.node };
									$$.node = new Node(EXPR, rhs);
									$$.node->type = $2.node->type;
									log(string(yytext) + " expr:left_parenthesis expr_E right_parenthesis", printFlag);
								}
						;

expr_E:					number expr_LR1				
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(EXPR_E, rhs);
									if(($2.node->rhs).size() != 0) {
										//Check compatibility of $1.node->type and ($2.node->rhs)[1].type
										bool doMatch = areCoercibleType($1.node->type, $2.node->type);
										$$.node->value = applyOperation($1.node , ($2.node->rhs[0]), ($2.node->rhs)[1]);
									} else 
										$$.node->value = $1.node->value;
									$$.node->type = $1.node->type;
									log(string(yytext) + " expr_E:	number expr_LR1	", printFlag);
								}
						| identifier expr_LR2 		
								{
									vector<Node *> rhs = { $1.node, $2.node };
									$$.node = new Node(EXPR_E, rhs);
									$$.node->type = variableTables[scope].getVariableType($1.node->name);
									log(string(yytext) + " expr_E:	identifier expr_LR2", printFlag);
								}
						| left_parenthesis expr_E right_parenthesis
								{
									vector<Node *> rhs = { $1.node, $2.node, $3.node };
									$$.node = new Node(EXPR, rhs);
									$$.node->type = $2.node->type;
								}
						
						| error		
								{
									vector<Node *> rhs = {};
									$$.node = new Node(EXPR_E, rhs);
									cerr << "Syntax error:	Missing number/identifier at line no. " << yylineno << endl;
								} 
						;

expr_LR1:				operator expr_E			
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(EXPR_LR1, rhs);
									log(string(yytext) + " expr_LR1: expr_E", printFlag);
								}
						
						| /* empty */               
								{
									vector<Node *> rhs = {};
									$$.node = new Node(EXPR_LR1, rhs);
									log(string(yytext) + " expr_LR1: empty", printFlag);
								}
						;

expr_LR2:				left_parenthesis param_list right_parenthesis /* Function call */			
								{
									vector<Node *> rhs = {$1.node, $2.node, $3.node};
									$$.node = new Node(EXPR_LR2, rhs);
									$$.node->type = t_func;
									//TODO:Use $$.node->name to query function table for function signature
									//Compare with param_list signature
									log(string(yytext) + " expr_LR2:left_parenthesis param_list right_parenthesis", printFlag);
								}
						| expr_LR1
								{
									vector<Node *> rhs = {$1.node};
									$$.node = new Node(EXPR_LR2, rhs);
									log(string(yytext) + " expr_LR2: expr_LR1", printFlag);
								}
						/* HANDLE SYNTAX ERROR */
						;

identifier: 			id id_LR                    
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(IDENTIFIER, rhs);
									$$.node->name = $1.node->name;
									$$.node->dimlist = $2.node->dimlist;
									log(string(yytext) + " identifier: id id_LR", printFlag);
								}
						;

identifier_E: 			id id_LR					
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(IDENTIFIER_E, rhs);
									$$.node->name = $1.node->name;
									$$.node->dimlist = $2.node->dimlist;
									log(string(yytext) + " identifier_E: id id_LR", printFlag);
								}
						| error						
								{
									vector<Node *> rhs = {};
									$$.node = new Node(IDENTIFIER_E, rhs);
									cerr << "Missing identifierpop at line no. " << yylineno;
								}	
						;

id_LR:					left_bracket  BR_DIMLIST1					
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(ID_LR, rhs);
									$$.node->dimlist = $2.node->dimlist;
									log(string(yytext) + " id_LR:BR_DIMLIST", printFlag);
								}	
						| /* empty */               
								{
									vector<Node *> rhs = {};
									$$.node = new Node(ID_LR, rhs);
									$$.node->dimlist = {};
									log(string(yytext) + " id_LR:empty", printFlag);
								}
						;

BR_DIMLIST1: 			expr_E right_bracket BR_DIMLIST_LR			
								{	
									vector<Node *> rhs = {$1.node, $2.node, $3.node};
									$$.node = new Node(BR_DIMLIST1, rhs);
									$$.node->dimlist = $2.node->dimlist;
									cerr << "Value  = " << $1.node->value;
									$$.node->dimlist.push_back(stoi($1.node->value)); //TODO: Check if expr.value is numeric
									log(string(yytext) + " BR_DIMLIST1: expr_E right_bracket BR_DIMLIST_LR", printFlag);
								}
		   				;

BR_DIMLIST_LR:			left_bracket BR_DIMLIST1					
								{
									vector<Node *> rhs = {$1.node, $2.node};
									$$.node = new Node(BR_DIMLIST_LR, rhs);
									$$.node->dimlist = $2.node->dimlist;
									log(string(yytext) + " BR_DIMLIST_LR:BR_DIMLIST", printFlag);
								}
						| /* empty */               
								{
									vector<Node *> rhs = {};
									$$.node = new Node(BR_DIMLIST_LR, rhs);
									$$.node->dimlist = {};
									log(string(yytext) + " BR_DIMLIST_LR:empty", printFlag);
								}
						;

operator: 				assignment_op			
								{
									vector<Node *> rhs = {$1.node};
									$$.node = new Node(OPERATOR, rhs);
									$$.node->value = $1.node->value;
									log(string(yytext) + " operator:assignment_op", printFlag);
								}
						| relational_op			
								{
									vector<Node *> rhs = {$1.node};
									$$.node = new Node(OPERATOR, rhs);
									$$.node->value = $1.node->value;
									log(string(yytext) + " operator:relational_op", printFlag);
								}
						| logical_op			
								{
									vector<Node *> rhs = {$1.node};
									$$.node = new Node(OPERATOR, rhs);
									$$.node->value = $1.node->value;
									log(string(yytext) + " operator:logical_op", printFlag);
								}
						| binary_op                     
								{
									vector<Node *> rhs = {$1.node};
									$$.node = new Node(OPERATOR, rhs);
									$$.node->value = $1.node->value;
									log(string(yytext) + " operator:binary_op", printFlag);
								}
						;
						
assignment_op:		 	'='	
						| MUL_ASSIGN			
								{
									vector<Node *> rhs = {};
									$$.node = new Node(ASSIGNMENT_OP, rhs);
									$$.node->value = string(yytext);
									log(string(yytext) + " ", printFlag);
								}
						| DIV_ASSIGN			
								{
									vector<Node *> rhs = {};
									$$.node = new Node(ASSIGNMENT_OP, rhs);
									$$.node->value = string(yytext);
									log(string(yytext) + " ", printFlag);
								}
						| MOD_ASSIGN			
								{
									vector<Node *> rhs = {};
									$$.node = new Node(ASSIGNMENT_OP, rhs);
									$$.node->value = string(yytext);
									log(string(yytext) + " ", printFlag);
								}
						| ADD_ASSIGN			
								{
									vector<Node *> rhs = {};
									$$.node = new Node(ASSIGNMENT_OP, rhs);
									$$.node->value = string(yytext);
									log(string(yytext) + " ", printFlag);
								}
						| SUB_ASSIGN			
								{
									vector<Node *> rhs = {};
									$$.node = new Node(ASSIGNMENT_OP, rhs);
									$$.node->value = string(yytext);
									log(string(yytext) + " ", printFlag);
								}
						| AND_ASSIGN			
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(ASSIGNMENT_OP, rhs);
									$$.node->value = string(yytext);
									log(string(yytext) + " ", printFlag);
								}
						| XOR_ASSIGN			
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(ASSIGNMENT_OP, rhs);
									$$.node->value = string(yytext);
									log(string(yytext) + " ", printFlag);
								}
						| OR_ASSIGN                     
								{
									vector<Node *> rhs = { $1.node };
									$$.node = new Node(ASSIGNMENT_OP, rhs);
									$$.node->value = string(yytext);
									log(string(yytext) + " ", printFlag);
								}
						;

relational_op:			'<'			{
										vector<Node *> rhs = {};
										$$.node = new Node(RELATIONAL_OP, rhs);
										$$.node->value = string(yytext);
									}
						| '>'		{
										vector<Node *> rhs = {};
										$$.node = new Node(RELATIONAL_OP, rhs);
										$$.node->value = string(yytext);
									}
						| LE_OP		{
										vector<Node *> rhs = {};
										$$.node = new Node(RELATIONAL_OP, rhs);
										$$.node->value = string(yytext);
									}
						| GE_OP 	{
										vector<Node *> rhs = {};
										$$.node = new Node(RELATIONAL_OP, rhs);
										$$.node->value = string(yytext);
									}
						;

logical_op:				'!'			{
										vector<Node *> rhs = {};
										$$.node = new Node(LOGICAL_OP, rhs);
										$$.node->value = string(yytext);
									}
						| AND_OP	{
										vector<Node *> rhs = {};
										$$.node = new Node(LOGICAL_OP, rhs);
										$$.node->value = string(yytext);
									}
						| OR_OP		{
										vector<Node *> rhs = {};
										$$.node = new Node(LOGICAL_OP, rhs);
										$$.node->value = string(yytext);
									}   
						;

binary_op:				'+'		{
									vector<Node *> rhs = {};
									$$.node = new Node(BINARY_OP, rhs);
									$$.node->value = string(yytext);
								}
						| '-'	{
									vector<Node *> rhs = {};
									$$.node = new Node(BINARY_OP, rhs); 
									$$.node->value = string(yytext);
								}
						| '*'	{
									vector<Node *> rhs = {};
									$$.node = new Node(BINARY_OP, rhs); 
									$$.node->value = string(yytext);
								}
						| '/'	{
									vector<Node *> rhs = {};
									$$.node = new Node(BINARY_OP, rhs); 
									$$.node->value = string(yytext);
								}
						| '%'	{
									vector<Node *> rhs = {};
									$$.node = new Node(BINARY_OP, rhs); //Magic number
									$$.node->value = string(yytext);
								}
						;

semi:					';' {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
							}
						| error		
								{
									cerr << "Missing semicolon at line no. " << yylineno;
								}		
						;

data_type_non_struct:	DATA_TYPE 	{	
										vector<Node *> rhs = {};
										$$.node = new Node(DATA_TYPE_NON_STRUCT, rhs);
										$$.node->type = string(yytext);
									}
						;

data_type_non_struct_E:	DATA_TYPE 	{	
										vector<Node *> rhs = {};
										$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
										$$.node->type = string(yytext);
									}
						| error		
								{
									cerr << "Missing data type at line no. " << yylineno;
								}
						;

id:						IDEN 		{ 
									vector<Node *> rhs = {};
									$$.node = new Node(ID, rhs);
									log(string(yytext) + " ", printFlag);
									$$.node->name = string(yytext);
								}
						;

number:					INTEGER 	{ 
										vector<Node *> rhs = {};
										$$.node = new Node(NUMBER, rhs);
										log(string(yytext) + " ", printFlag);
										$$.node->type = t_int;
										$$.node->value = string(yytext);
									}
						| FLOATING_POINT 	{ 
												vector<Node *> rhs = {};
												$$.node = new Node(NUMBER, rhs);
												log(string(yytext) + " ", printFlag);
												$$.node->type = t_float;
												$$.node->value = string(yytext);
											}
						| CHAR_LIT	{
										vector<Node *> rhs = {};
										$$.node = new Node(NUMBER, rhs);
										$$.node->type = t_char;
										$$.node->value = string(yytext);
									}
						;

left_parenthesis:		'(' {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
							}
						;

left_parenthesis_E:		'('	 {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
							 }
						| error 	
								{
									cerr << "Missing ( at line no. " << yylineno;
								}
						;

right_parenthesis:		')' {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
							}
						| error 	
								{
									cerr << "Missing ) at line no. " << yylineno;
								}
						;

left_brace:				'{' {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
								scopepp();
							}
						;

left_brace_E:			'{' {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
								scopepp();
							}
						| error 	
								{
									cerr << "Missing { at line no. " << yylineno;
								}		
						;

right_brace:			'}' {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
								scopemm();
							}
						| error 	
								{
									cerr << "Missing } at line no. " << yylineno;
								}		
						;

left_bracket:			'[' {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
							}			
						;

right_bracket:			']' {	
								vector<Node *> rhs = {};
								$$.node = new Node(DATA_TYPE_NON_STRUCT_E, rhs);
							}			
						| error 	
								{
									cerr << "Missing ] at line no. " << yylineno;
								}		
						;

%%

int main() {
	yyparse();
	printSymbolTables();
	printFunctionTable();
	return 0;
}