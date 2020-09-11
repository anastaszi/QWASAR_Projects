// convert expressiont into the expression tree
// solve the tree

// implement Stack class
// implement ExpressionTree class
// implement Solver class

/*
    main algo for expression tree conversion
    tree_node = new Node
    stack <- tree_node
    current <- stack.top
    while i in expression
      if i is '('
          current.left <- new Node
          stack <- current
          current <- current.left
      else if i is operator (-/+/...)
          current.value <- i
          current.right <- new Node
          stack <- current
          current <- current.right
      else if i is operand (number)
          current.value <- i
          current <- stack.pop
      else if i is ')'
          current <- stack.pop
    return tree_node


    https://runestone.academy/runestone/books/published/pythonds/Trees/ParseTree.html
*/
