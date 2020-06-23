require 'sqlite3'

class User
  attr_accessor :db

  def initialize
    @db = SQLite3::Database.open "my_first_app.db"
    @db.execute "CREATE TABLE IF NOT EXISTS users(Id INTEGER PRIMARY KEY AUTOINCREMENT,
        firstname TEXT,
        lastname TEXT,
        age INT,
        password TEXT,
        email TEXT UNIQUE)"
  end

  def create(firstname, lastname, age, email, password)
    begin
      @db.execute "INSERT INTO users(firstname, lastname, age, password, email) VALUES(?, ?, ?, ?, ?)", [firstname, lastname, age.to_i, password, email]
      p get(@db.last_insert_row_id)
      return @db.last_insert_row_id
    rescue  SQLite3::Exception => e
      return nil
    end
  end

  def get(user_id)
    user = Hash.new()
    stm = @db.prepare("SELECT * FROM users WHERE rowid=?")
    stm.bind_params(user_id.to_i)
    rs = stm.execute.next
    if rs != nil
      stm.columns.each_with_index do |col, i|
        user[col] = rs[i]
      end
    end
    return user
  end

  def sign_in(email)
    user = Hash.new()
   stm = db.prepare("SELECT rowid,email,password FROM users WHERE email='" + email + "'")
   rs = stm.execute.next
   if rs != nil
     stm.columns.each_with_index do |col, i|
       user[col] = rs[i]
     end
     return user
   end
   return nil
  end

  def all
    all_users = []
    stm = @db.prepare "SELECT firstname,lastname,age,email FROM users"
    rs = stm.execute
    rs.each do |row|
      user = Hash.new()
      stm.columns.each_with_index do |col, i|
        user[col] = row[i]
      end
      all_users.push(user)
    end
    return all_users
  end

  def update(user_id, attribute, value)
    stm = @db.prepare("UPDATE users SET " + attribute + "=? WHERE rowid=?")
    stm.bind_params(value, user_id.to_i)
    stm.execute
    return get(user_id)
  end

  def destroy(user_id)
    stm = @db.prepare("DELETE FROM users WHERE rowid=?")
    stm.bind_params(user_id.to_i)
    stm.execute
  end

  def close
    @db.close if @db
  end

end
