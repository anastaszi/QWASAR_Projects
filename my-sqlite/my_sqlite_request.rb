require 'csv'
class MySqliteRequest
  attr_accessor :request_method,
                :file_name,
                :data,
                :headers,
                :operations,
                :file_name_join

  def initialize
    @request_method = nil
    @file_name = nil
    @operations = {select: nil, where: nil, values: nil, delete: nil, order: nil, set: nil, join: nil}
  end

# parse src methods

  def from(table_name)
    if @request_method.nil?
      add_request_method('from', table_name)
    else
      puts "ERROR: You can not use FROM method together with #{@request_method.upcase} method."
    end
    return self
  end

  def insert(table_name)
    if @request_method.nil?
      add_request_method('insert', table_name)
    else
      puts "ERROR: You can not use INSERT INTO method together with #{@request_method.upcase} method."
    end
    return self
  end

  def update(table_name)
    if @request_method.nil?
      add_request_method('update', table_name)
    else
      puts "ERROR: You can not use UPDATE method together with #{@request_method.upcase} method."
    end
    return self
  end

# parse operation methods

  def select(column)
    if (column == nil || column.length == 0)
      puts "ERROR: You need to select columns."
    elsif !column.kind_of?(Array)
      @operations[:select] = ["Id", column]
    else
      @operations[:select] = ["Id"] + column
    end
    return self
  end

  def where(column_name, criteria)
    if !column_name.nil? && !criteria.nil?
      @operations[:where] = {column: column_name, criteria: criteria}
    else
      puts "ERROR: Wrong format for WHERE method. Condition wasn't applied."
    end
    return self
  end

  def order(order, column_name)
    if ![":desc", ":acs"].index(order) || column_name.nil?
      puts "ERROR: Wrong format for ORDER method. Data wasn't sorted."
    elsif (!@operations[:order])
      @operations[:order] = [{column: column_name, order: order == ":desc"}]
    else
      @operations[:order].push({column: column_name, order: order == ":desc"})
    end
    return self
  end

  def values(data)
    if @request_method != 'insert'
      puts "ERROR: You can not use VALUES method without INSERT INTO method."
    elsif data.nil?
      puts "ERROR: No data provided."
    else
      @operations[:values] = data
    end
    return self
  end

  def set(data)
    if @request_method != 'update'
      puts "ERROR: You can not use SET method without UPDATE method."
    elsif data.nil?
      puts "ERROR: No data provided."
    else
      @operations[:set] = data
    end
    return self
  end

  def delete
    @operations[:delete] = true
    return self
  end

  def join(column_on_db_a, filename_db_b, column_on_db_b)
    if (column_on_db_a.nil? || column_on_db_b.nil? || filename_db_b.nil?)
      puts "ERROR: Wrong input to JOIN method"
      @data = nil
    elsif !File.file?(filename_db_b)
      puts "ERROR: No [#{filename_db_b}] file found. JOIN would not be applied"
      @data = nil
    elsif !@headers.index(column_on_db_a)
      puts "ERROR: No column [#{column_on_db_a}] in [#{@file_name}] found"
      @data = nil
    else
      @operations[:join] = {col_a: column_on_db_a, col_b: column_on_db_b }
      @file_name_join = filename_db_b
    end
    return self
  end


  def run
    if @request_method == "from" && !(@file_name.nil?)
      if @operations[:delete]
        run_delete
      elsif @operations[:select]
        run_select
        return data_to_hash
      else
        puts "ERROR: Wrong request format for FROM method"
      end
    elsif (@request_method == "update" && !(@file_name.nil?))
      run_update
    elsif @request_method == "insert" && !(@file_name.nil?)
      run_insert
    else
      puts "ERROR: Your didn't provide input data"
    end
  end

  #private methods

  def add_request_method(method, table_name)
    @request_method = method
    if (table_name && File.file?(table_name))
      @file_name = table_name
      file_arr = File.read(table_name).split("\n");
      file_str = CSV.generate do |csv|
        csv << ['Id'] + file_arr.shift.split(",")
        file_arr.map.with_index do |row, i|
          csv << [i] + row.split(",")
        end
      end
      @data = CSV.parse(file_str, headers: true)
      if @data
        @headers = @data.headers
      end
    else
      puts "ERROR: No [#{table_name}] was file found."
    end
  end

  def update_src(array, header)
    temp_str = CSV.generate do |csv|
      csv << header
      array.each do |row|
        csv << row
      end
    end
    @data = CSV.parse(temp_str, headers: true);
    @headers = @data.headers
  end

  def write_to_file(array, header)
    CSV.open(@file_name, "w") do |csv|
      csv << @headers[1..(@headers.length - 1)]
      array.map do |row|
        if row
          temp_row = row
          temp_row.delete(0)
          csv << temp_row
        end
      end
    end
  end

  def data_to_hash
    if @data
      array = @data.map do |row|
        temp_row = row
        temp_row.delete(0)
        temp_row.to_h
      end
      return array
    end
  end

  def run_select
    # run join method
    if @operations[:join]
      run_join
    end
    # apply where conditions
    if @operations[:where] && @data
      temp_array = @data.select do |row|
        row[@operations[:where][:column]] == @operations[:where][:criteria]
      end
      update_src(temp_array, @headers)
    end
    # select columns
    if @operations[:select].include? "*"
      @operations[:select] = @headers
    end
    if @data
      temp_array = @data.map do |row|
          row.values_at(*@operations[:select])
      end
      update_src(temp_array, @operations[:select])
    end
    # run order
    while (@data && @operations[:order] && @operations[:order].length > 0)
      order = @operations[:order].pop
      column_index = @headers.find_index(order[:column])
      temp_array = @data.sort_by {|row| row[column_index]}
      if (order[:order])
        temp_array = temp_array.reverse
      end
      update_src(temp_array, @headers)
    end
  end

  def run_update
    if !@operations[:set].nil?
      temp_array = @data.map do |row|
        # update row if 'where' method was provided and row matches criteria from 'where' method
        #            or 'where' method was not provided at all [it means that all rows should be updated]
        temp_where = @operations[:where]
        temp_condition = (temp_where && row[temp_where[:column]] == temp_where[:criteria]) || !temp_where
        if temp_condition
          @operations[:set].each do |column, value|
            if @headers.index(column)
              row[column] = value
            end
          end
        end
        row
      end
      write_to_file(temp_array, @headers)
    else
      puts "ERROR: No SET method was provided"
    end
  end

  def run_insert
    if !@operations[:values].nil?
      row_to_add = @operations[:values].split(",")
      CSV.open(@file_name, "ab") do |csv|
        if row_to_add
          csv << row_to_add
        end
      end
    end
  end

  def run_delete
    if @request_method == 'from'
      temp_array = @data.map do |row|
        if @operations[:where] && row[@operations[:where][:column]] != @operations[:where][:criteria]
          row
        end
      end
      write_to_file(temp_array, @headers)
    else
      puts "ERROR: You can not use DELETE method without FROM method"
    end
  end

  def run_join
    data_from_b = CSV.parse(File.read(@file_name_join), headers: true)
    if data_from_b && @data
      headers_from_b = data_from_b.headers.map {|elem| @file_name_join.split('.')[0] + "."+ elem}
      joined_headers = @headers + headers_from_b
      new_data = []
      @data.map do |row|
        data_from_b.map do |row_b|
          if row[@operations[:join][:col_a]] == row_b[@operations[:join][:col_b]]
            new_data.push(row.values_at(*@headers) + row_b.values_at(*data_from_b.headers))
          end
        end
      end
      update_src(new_data, joined_headers)
    end
  end

  private :update_src, :run_select, :run_insert, :run_update, :run_delete, :run_join, :add_request_method
end
