
#!/usr/bin/env ruby

require 'sinatra'
require_relative 'my_user_model'

set :bind, '0.0.0.0'
set :port, 8080
set :session, Hash.new
set :user, User.new

enable :sessions

helpers do
  def authorized?(user_data)
      @auth ||=  Rack::Auth::Basic::Request.new(request.env)
      user_data and @auth.provided? and @auth.basic? and @auth.credentials and @auth.credentials.to_a == [user_data['email'], user_data['password']]
  end

  def protected!(user_data)
      return if authorized?(user_data)
      headers['WWW-Authenticate'] = 'Basic realm="Restricted Area"'
      halt 401, "Not authorized\n"
  end
end

    get /\/users|\// do
      @all_users = settings.user.all
      erb :index
    end

    post '/users' do
      if (settings.user.create(params[:firstname], params[:lastname], params[:age], params[:email], params[:password]) == nil)
        return  "This Email is already taken"
      end
    end

    post '/sign_in' do
      email = Rack::Auth::Basic::Request.new(request.env)
      if (email.provided?)
        user_data = settings.user.sign_in(email.username)
        if (user_data)
          protected!(user_data)
          session[:logged_user] = user_data['Id']
         return "You are in!"
       end
      end
      return "No user with this email"
    end

    put '/users' do
      user = session[:logged_user]
      if user && params[:password]
          settings.user.update(user, 'password', params[:password])
          return settings.user.get(user).to_s
      end
    end

    delete '/sign_out' do
      if session[:logged_user]
        session.delete(:logged_user)
        return "You are out!"
      end
      return "You should be logged in to log out"
    end

    delete '/users' do
      user = session[:logged_user]
      if user
        session.delete(:logged_user)
        settings.user.destroy(user)
        return "User info is deleted!"
      end
      return "You should be logged in to delete user info!"
    end


  #curl -X POST -i http://web-hfaaae969-02.docode.qwasar.io/users  -d "firstname=value1" -d "lastname=value2" -d "age=34" -d "email=an" -d "password=nfdf"
  #curl -X GET -i http://web-hfaaae969-02.docode.qwasar.io/users
