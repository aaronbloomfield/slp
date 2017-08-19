FROM ubuntu:16.04
MAINTAINER Aaron Bloomfield "aaron@virginia.edu"

# step 1: a few things that are needed for this script to work well in docker
RUN DEBIAN_FRONTEND=noninteractive apt-get update -y -q -q
RUN DEBIAN_FRONTEND=noninteractive apt-get dist-upgrade -y -q -q
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q supervisor net-tools locales apt-utils
RUN locale-gen en_US.UTF-8

# step 2: install some packages
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q g++ emacs24 vim astyle tofrodos source-highlight gdb git sqlite3

# step 3: set up a few sanity aliases in .bashrc
RUN echo "alias mv='mv -i'" >> ~/.bashrc
RUN echo "alias rm='rm -i'" >> ~/.bashrc
RUN echo "alias cp='cp -i'" >> ~/.bashrc

# step 4: set up Ruby on Rails, as per the directions at
# https://gorails.com/setup/ubuntu/16.04; as of August 2017, this
# installs Ruby version 2.4.1 and Rails version 5.1.3
RUN DEBIAN_FRONTEND=noninteractive apt-get update -y -q -q
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q git-core curl zlib1g-dev build-essential libssl-dev libreadline-dev libyaml-dev libsqlite3-dev sqlite3 libxml2-dev libxslt1-dev libcurl4-openssl-dev python-software-properties libffi-dev
RUN git clone https://github.com/rbenv/rbenv.git ~/.rbenv
RUN echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
RUN echo 'eval "$(rbenv init -)"' >> ~/.bashrc
RUN git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
RUN echo 'export PATH="$HOME/.rbenv/plugins/ruby-build/bin:$PATH"' >> ~/.bashrc
RUN sed -i s/shopt/\#shopt/g ~/.bashrc
RUN . ~/.bashrc; rbenv install 2.4.1
RUN . ~/.bashrc; rbenv global 2.4.1
RUN . ~/.bashrc; ruby -v
RUN . ~/.bashrc; gem install --no-ri --no-rdoc bundler
RUN curl -sL https://deb.nodesource.com/setup_8.x | bash -
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q nodejs
RUN . ~/.bashrc; gem install --no-ri --no-rdoc rails -v 5.1.3
RUN . ~/.bashrc; rbenv rehash
RUN . ~/.bashrc; rails -v
RUN sed -i s/\#shopt/shopt/g ~/.bashrc

# step 5: set up Python and Django; as of August 2017, this is Python
# version 3.x and Django version 1.11.x
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q python3-pip
RUN pip3 install --upgrade pip
RUN pip install Django==1.11
RUN pip install virtualenv

# step 6: create default projects, as that will download the necessary files
RUN sed -i s/shopt/\#shopt/g ~/.bashrc
RUN . ~/.bashrc; cd ~; rails new myapp
RUN sed -i "s/gem 'tzinfo-data'/#gem 'tzinfo-data'/g" ~/myapp/Gemfile
RUN echo "gem 'tzinfo-data'" >> ~/myapp/Gemfile
RUN /bin/rm -f ~/myapp/Gemfile.lock
RUN . ~/.bashrc; cd ~/myapp; bundle install
RUN /bin/rm -rf ~/myapp
RUN sed -i s/\#shopt/shopt/g ~/.bashrc

# step 7: create work directory and expose the ports used with the development servers
RUN mkdir /root/work
EXPOSE 3000

# all done!

