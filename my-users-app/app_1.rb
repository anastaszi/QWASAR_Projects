
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
      @session = session[:logged_user]
      @error_sign_up = session[:error_sign_up]
      @error_log_in = session[:error_log_in]
      erb :index_1
    end

    post '/users' do
      session.delete(:error_sign_up)
      if (settings.user.create(params[:firstname], params[:lastname], params[:age], params[:email], params[:password]) == nil)
         session[:error_sign_up] = "This email is already taken!"
      end
      redirect '/users'
    end

    post '/sign_in' do
      session.delete(:log_in)
      user_data = settings.user.sign_in(params[:email])
      if (user_data && user_data['password'] == params[:password])
        session[:logged_user] = user_data['Id']
      else
        session[:error_log_in] = "Wrong email or password. Try again!"
      end
      redirect '/users'
    end

    put '/users' do
      user = session[:logged_user]
      if user && params[:password]
          settings.user.update(user, 'password', params[:password])
      end
      redirect '/users'
    end

    delete '/sign_out' do
      if session[:logged_user]
        session.delete(:logged_user)
        redirect '/users'
      end
      return "You should be logged in to log out"
    end

    delete '/users' do
      user = session[:logged_user]
      if user
        session.delete(:logged_user)
        settings.user.destroy(user)
        redirect '/users'
      end
      return "You should be logged in to delete user info!"
    end


  #curl -X POST -i http://web-hfaaae969-02.docode.qwasar.io/users  -d "firstname=value1" -d "lastname=value2" -d "age=34" -d "email=an" -d "password=nfdf"
  #curl -X GET -i http://web-hfaaae969-02.docode.qwasar.io/users
