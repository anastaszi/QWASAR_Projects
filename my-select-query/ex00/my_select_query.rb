require 'csv'
class MySelectQuery
  attr_accessor :data, :headers
  def initialize(csv_content)
    @data = CSV.parse(csv_content, headers: true)
    @headers = @data.headers
  end

  def where(column_name, criteria)
    result = []
    @data.select{|row| row[column_name]  == criteria}.each do |elem|
      subresult = []
      @headers.each {|column| subresult.push(elem[column])}
      result.push(subresult.join(","))
    end
    return result
  end

end

a = MySelectQuery.new(File.open("nba_player_data.csv"))
p a.where("position", "F")
