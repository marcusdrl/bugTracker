# bugTracker

Este projeto foi criado com o intuito de demonstrar um pouco do que eu aprendi com QT e C++, exclusivamente para utilizar como portfólio. Front end não é o meu forte então não se assustem se acharem a UI um pouco "diferente".

<h1>COMO USAR</h1>
Primeiramente para se conectar a aplicação você terá de fazer o download dela (aqui). Após realizar o download e a instalação, você irá abrir a aplicação e 
irá se registrar com uma conta, preenchendo corretamente os campos porque sua conta terá que ser ativada, então cuidado nesta parte.

![cadastrar](https://user-images.githubusercontent.com/71731401/133265459-5c05a971-c51e-4c8b-ba27-0c72b679c4f9.png)

### IMPORTANTE
Ao registrar sua conta você deverá escolher um cargo de acesso(este pode ser modificado dentro do próprio app, com algumas condições). Cargos são de extrema importância pois eles te limitam dentro do que você pode fazer ou acessar dentro da aplicação, vou tentar resumir a diferença entre os cargos abaixo:
* <p align="justify"><strong>ADMINISTRADOR (ADMIN):</strong> Você deverá escolher este cargo quando quiser trabalhar em aplicações pessoais, ou caso você realmente tenha um empresa na qual você possui funcionários os quais necessitam de alguém para gerenciá-los. Como administrador você poderá aceitar novos membros na sua equipe pela aba de aplicações, delegar funções(cargo 'Manager'), criar projetos, tickets e etc;</p>
* <p align="justify"><strong>DESENVOLVEDOR SR./JR.:</strong> Os cargos de desenvolvedores não possuem nada de especial, eles somente podem visualizar os projetos em que se encontram, visualizam tickets e podem editar status de tickets(caso já tenha sido resolvido);</p>
* <p align="justify"><strong>MANAGER:</strong> Primeiramente manager não é um cargo de cadastro, é uma função que deve ser delegada por alguém(ADMINISTRADOR) que já possua uma empresa, inicialmente a pessoa que for receber o cargo de MANAGER deverá se cadastrar como DESENVOLVEDOR e posteriormente receber sua "promoção";</p>

Com a sua conta criada você agora irá ativar ela pelo link que você recebeu em seu e-mail para que esteja apto a realizar o login.
Após realizar o login esta será a primeira imagem que você terá de dentro da aplicação:

![menu_esquerda](https://user-images.githubusercontent.com/71731401/133266115-030a4d38-f653-41db-8326-ad248eb72280.png)

<p align='justify'>Na esquerda é possível visualizar o menu com todas as opções de usuário, e a direita fica a nossa página inicial (home) do projeto, que sempre irá representar para
o usuário que está acessando os últimos tickets (se for ADMIN será os ultimos tickets da empresa tickets pessoais inclusos, se for MANAGER será os ultimos tickets dos projetos que gerencia juntamente com os tickets pessoais, se for DESENVOLVEDOR somente os tickets pessoais destinados a você irão aparecer) juntamente do resumo do status de prioridade dos tickets(alta, média, baixa). O menu irá variar de acordo com o nível de acesso do usuário, por exemplo a imagem acima representa o menu de um ADMINISTRADOR, ou seja o usuário que na hora do cadastro resolveu criar sua própria empresa para gerenciar seu projetos, sejam eles pessoais ou não.</p>

<strong>As opções de menu são auto-explicativas, e para evitar que este fique muito extenso e cansativo de ler, irei partir para a explicação das funções mais importantes em minha visão:</strong>
1. **ABRIR TICKETS:** É a função principal da aplicação, aqui é possivel controlar bugs ou possiveis modificações que forem necessárias e designar para alguém em específico que tome conta do problema. A aba para escolher desenvolvedores irá depender da aba projetos, quando for feita a escolha do projeto todos os desenvolvedores que estiverem registrados nele irão aparecer, também é possível adicionar um arquivo de anexo(até 16mb) este sendo opcional.

![abrir_ticket](https://user-images.githubusercontent.com/71731401/133286039-8af286d3-cf2e-4c42-bf66-8cb797580dda.png)

2. **ABA DE TICKETS:** Todas os botões de exibição de tickets('Página Inicial', 'Meus Tickets', 'Tickets da Equipe', 'Meus Tickets Solucionados', 'Tickets Solucionados(Equipe)') possuem função de double-click, esta por sua vez irá abrir uma aba detalhada do ticket que você selecionou(com os dados de abertura, status, prioridade e etc):

![double_click_ticket](https://user-images.githubusercontent.com/71731401/133273174-ea53ad40-db54-4b55-b8be-e6c6892b495e.png)

![detalhes_tickets](https://user-images.githubusercontent.com/71731401/133284846-5a13b360-32e4-490e-bb31-c97bd50ee4e0.png)
**Acima é possivel visualizar os detalhes do ticket como nº do ticket, quem abriu ele, titulo, status, comentários, histórico de modificações e etc...**

  2.1 **COMENTÁRIOS**: É possível adicionar um novo comentário que será adicionado a aba detalhada do ticket, este pode ser usado para "comunicação" caso seja necessária, ou então para registrar o progresso da solução do problema.
