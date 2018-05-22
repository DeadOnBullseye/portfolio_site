Rails.application.routes.draw do
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html

  root 'pages#index', as: 'welcome'

  get 'home' => 'pages#home', as: 'home'

  get 'about' => 'pages#about', as: 'about'

  get 'skills' => 'pages#skills', as: 'skills'

  get 'projects' => 'pages#projects', as: 'projects'

  get 'contact' => 'pages#contact', as: 'contact'

  get 'thanks' => 'pages#thanks', as: 'thanks'

end
