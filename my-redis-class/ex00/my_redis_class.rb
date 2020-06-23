class MyRedisClass
  attr_accessor :data
  def initialize()
    @data = Hash.new()
  end

  def my_set(key, value)
    @data[key.to_s] = value
  end

  def my_get(key)
    if @data.empty?
      return nil
    end
    return @data[key.to_s]
  end

  def my_mset(pairs)
    if pairs != nil
      pairs.each {|elem| my_set(elem[0].to_s, elem[1]) }
    end
  end

  def my_mget(keys)
    result = []
    if keys != nil
      keys.each {|elem| result.push(my_get(elem.to_s))}
    end
    return result
  end

  def my_del(keys)
    if keys != nil
      keys.each {|elem| @data.delete(elem.to_s)}
    end
  end

  def my_exists(key)
    return @data.has_key?(key.to_s)
  end

  def my_rename(key, new_key)
    if !my_exists(key.to_s) || my_exists(new_key.to_s)
      return false
    end
    my_set(new_key.to_s, my_get(key.to_s))
    my_del([key.to_s])
    return true
  end

  def backup()
    File.open("my_dump.rdb", "w") {|f| @data.each {|key, value|
      f.write "#{key}->#{value}\n"}}
  end

  def restore()
    file_data = File.read("my_dump.rdb").split("\n")
    file_data.each do |line|
      pair = line.split("->")
      my_set(pair[0], pair[1])
    end
  end

end
