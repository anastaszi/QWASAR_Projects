require 'readline'
require_relative 'my_sqlite_request'

def request_parser(buf)
  valid_methods = ["SELECT", "FROM", "INSERT", "UPDATE", "VALUES", "SET", "JOIN", "WHERE", "ORDER", "DELETE", "ON"]
  array = buf.strip.split(/\s*\(|\s*=\s*|[ ]{1,}|\)*;|,\s*|\)/)
  request = Hash.new
  while (array.length > 0)
    if valid_methods.index(array[0])
      command = array[0].downcase
      request[command] = []
      if array[0] == "INSERT" || array[0] == "ORDER"
        array.shift
      end
      array.shift
      while(array.length > 0 && !valid_methods.index(array[0]))
        request[command].push(array[0])
        array.shift
      end
    else
      puts "ERROR: Bad request format. Start from SELECT/DELETE/UPDATE/INSERT INTO"
      return nil
    end
  end
  return request
end

def run_request(commands)
  if commands
    request = MySqliteRequest.new
    commands.each do |method, params|
      case method
      when "from", "insert", "update"
        # my_sqlite_cli request awaits call like FROM table_name
        # my_sqlite_cli request awaits call like INSERT table_name
        # my_sqlite_cli request awaits call like UPDATE table_name
        request.method(method).call(params[0])
      when "select"
        # my_sqlite_cli request awaits call like SELECT *
        # or SELECT column1
        # or SELECT column1, column2
        if params[0] == "*"
          request.method(method).call("*")
        else
          request.method(method).call(params)
        end
      when "join"
        # my_sqlite_cli request awaits call like JOIN table_name ON column_a = column_b
        request.method(method).call(commands["on"][0], params[0], commands["on"][1])
      when "where"
        # my_sqlite_cli request awaits call like WHERE column_name = condition
        request.method(method).call(params[0], params[1])
      when "delete"
        # my_sqlite_cli request awaits call like DELETE
        request.method(method).call
      when "order"
        # my_sqlite_request order method prototyped as order(order, column_name)
        # order_parametr could be (":asc" or ":desc")
        # my_sqlite_cli request awaits call like ORDER BY column_name DECS
        if params[1]
          order = ":" + params[1].downcase
        else
          order = nil
        end
        request.method(method).call(order, params[0])
      when "set"
        # my_sqlite_cli request awaits call like SET column1 = val1, column2 = val2 ...
        request.method(method).call(array_to_hash(params))
      when "values"
        # my_sqlite_cli request awaits call like VALUES (val1, val2, val3,...)
        request.method(method).call(params.join(","))
      end
    end
    return request.run
  end
end

def array_to_hash(array)
  result = Hash.new
  array.each.with_index do |val, i|
    if i % 2 == 0
      result[val] = array[i + 1]
    end
  end
  return result
end

def hash_to_array(hash_array)
  big_array = []
  hash_array.each do |row|
    sm_array = []
    row.each_value{|value| sm_array.push(value)}
    big_array.push(sm_array.join('|'))
  end
  return big_array
end

def cli
  puts "MySQLite version 0.3 2020-05-23"
  loop do
    buf = Readline.readline("my_sqlite_cli> ", true)
    if buf == "quit"
      break
    end
    request = request_parser(buf)
    result = run_request(request)
    if (request && request["select"] && result.kind_of?(Array))
      hash_to_array(result).each {|row| puts row}
    end
  end
end

cli()
