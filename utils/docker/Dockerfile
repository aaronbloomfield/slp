FROM ubuntu:16.04
MAINTAINER Aaron Bloomfield "aaron@virginia.edu"

# step 1: a few things that are needed for this script to work well in docker
RUN DEBIAN_FRONTEND=noninteractive apt-get update -y -q -q
RUN DEBIAN_FRONTEND=noninteractive apt-get dist-upgrade -y -q -q
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q supervisor net-tools locales apt-utils software-properties-common
RUN locale-gen en_US.UTF-8

# step 2: install some necessary packages
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q g++ emacs24 vim astyle tofrodos source-highlight gdb git sqlite3

# step 3: set up a few sanity aliases in .bashrc
RUN echo "alias mv='mv -i'" >> /root/.bashrc
RUN echo "alias rm='rm -i'" >> /root/.bashrc
RUN echo "alias cp='cp -i'" >> /root/.bashrc

# step 4: set up Ruby, as per the directions at
# https://www.brightbox.com/blog/2017/01/13/ruby-2-4-ubuntu-packages/;
# as of August 2017, that is Ruby version 2.4.1
RUN DEBIAN_FRONTEND=noninteractive apt-add-repository -y ppa:brightbox/ruby-ng
RUN DEBIAN_FRONTEND=noninteractive apt-get update -y -q -q
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q ruby2.4 ruby2.4-dev
RUN ruby2.4 -v
RUN update-alternatives --set ruby /usr/bin/ruby2.4
RUN update-alternatives --set gem /usr/bin/gem2.4
RUN ruby -v

# step 5: install rails and other gems; as of August 2017, that is
# Rails version 5.1.3 (which is explicitly installed)
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q zlib1g-dev pkg-config libsqlite3-dev # needed by various gems
RUN echo gem: --no-ri --no-rdoc > /root/.gemrc
RUN gem install bundler
RUN gem install nokogiri
RUN gem install rails -v 5.1.3

# step 6: create a rails app so that it downloads all the appropriate
# gems
RUN cd /root; rails new myapp
RUN sed -i "s/gem 'tzinfo-data'/#gem 'tzinfo-data'/g" /root/myapp/Gemfile
RUN echo "gem 'tzinfo-data'" >> /root/myapp/Gemfile
RUN /bin/rm -f /root/myapp/Gemfile.lock
RUN cd /root/myapp; bundle install
RUN /bin/rm -rf /root/myapp

# step 7: install nodejs, which is needed by the `rails server` comamnd
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q curl
RUN curl -sL https://deb.nodesource.com/setup_8.x | bash -
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q nodejs

# step 8: set up Python and Django; as of September 2017, this is Python
# version 3.5.2 and Django version 1.11.5 (which is explicitly installed)
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y -q -q python3-pip
RUN pip3 install --upgrade pip
RUN pip install Django==1.11.5
RUN pip install virtualenv

# step 9: create work directory and expose the ports used with the
# development servers
RUN mkdir /root/work
EXPOSE 3000 8000

# step 10: clean up
RUN DEBIAN_FRONTEND=noninteractive apt-get clean

# all done!
